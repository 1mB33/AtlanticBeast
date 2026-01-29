#ifndef AB_VOXEL_GRID_H
#define AB_VOXEL_GRID_H

#include "Vulkan/MemoryUploadTracker.hpp"
#include "Raycaster/Voxel.hpp"
#include "Primitives/ColoredCube.hpp"

namespace B33::Rendering
{

class IWorldGrid : public ::B33::Rendering::MemoryUploadTracker
{
protected:

    static constexpr size_t DefaultVoxelGridDim = 64;

public:

    explicit IWorldGrid(size_t uGridWidth = DefaultVoxelGridDim)
        : m_uGridDim(uGridWidth)
        , m_VoxelGrid(uGridWidth * uGridWidth * uGridWidth)
    { }

public:

    const void* GetGridPtr() const
    { return m_VoxelGrid.data(); }

    ::std::vector<::B33::Rendering::Voxel>& GetGrid()
    { return m_VoxelGrid; }

    const ::std::vector<Voxel>& GetGrid() const
    { return m_VoxelGrid; }

    size_t GetVoxelsSizeInBytes() const
    { return m_VoxelGrid.size() * sizeof(Voxel); }

    ::size_t GetGridWidth() const
    { return m_uGridDim; }

    ::size_t GetVoxels() const
    { return m_VoxelGrid.size(); }

    virtual const void* GetObjectsPtr() const = 0;
    
    virtual ::size_t GetObjectsSizeInBytes() const = 0;

    virtual ::size_t GetUsedObjectsSizeInBytes() const = 0;

public:

    BEAST_API void SetVoxel(const ::B33::Math::iVec3& pos, uint32_t uColor);

public:

    virtual bool CheckIfVoxelOccupied(const ::B33::Math::iVec3& pos) const = 0;

protected:

    BEAST_API ::size_t CalcIndex(const ::B33::Math::iVec3& pos) const;

    BEAST_API void PlaceOnGrid(const ::B33::Math::iVec3& pos,
                               const ::B33::Math::iVec3& area,
                               const ::size_t uId);

    BEAST_API void RemoveFromGrid(const ::B33::Math::iVec3& pos, 
                                  const ::B33::Math::iVec3& area,
                                  const ::size_t uId);

private:

    ::size_t m_uGridDim = -1;
    ::std::vector<::B33::Rendering::Voxel> m_VoxelGrid;

};

template<class StoredObjectType>
class WorldGrid : public IWorldGrid
{
public:

    explicit WorldGrid(size_t uGridWidth = IWorldGrid::DefaultVoxelGridDim)
        : IWorldGrid(uGridWidth)
        , m_StoredObjects(this->GetVoxelsSizeInBytes() / sizeof(Voxel))
        , m_uObjectsCount(0)
    { }

public:

    virtual const void* GetObjectsPtr() const override 
    { return m_StoredObjects.data(); }
    
    virtual size_t GetObjectsSizeInBytes() const override 
    { return m_StoredObjects.size() * sizeof(StoredObjectType); }
    
    virtual size_t GetUsedObjectsSizeInBytes() const override 
    { return (m_uObjectsCount + 1) * sizeof(StoredObjectType); }

    StoredObjectType& GetById(size_t uId) 
    {
        AB_ASSERT(uId < m_uObjectsCount);
        return m_StoredObjects[uId]; 
    }

public:

    virtual bool CheckIfVoxelOccupied(const ::B33::Math::iVec3& pos) const override
    { 
        const ::std::vector<Voxel>& voxelsGrid = this->GetGrid();
        const size_t uDim = this->GetGridWidth(); 
        const size_t uIndex = CalcIndex(pos);

        AB_ASSERT(uIndex < voxelsGrid.size());

        if (voxelsGrid[uIndex].Type == 0)
            return false;

        for (::uint32_t i = 0; i < voxelsGrid[uIndex].Type; ++i)
            if (::B33::Math::iVec3::ToiVec3(m_StoredObjects[voxelsGrid[uIndex].Id[i]].GetPosition()) == pos) 
                return true;
        
        return false;
    }

public:

    template<class U>
    size_t GenerateObjectAtVoxel(const ::B33::Math::iVec3& pos, U&& sot)
    {
        size_t uId = GenerateObject(pos, this->GetGrid(), ::std::forward<U>(sot));
        this->ForceUpload();
        return uId;
    }

    void RemoveObject(const size_t uObjectId)
    {
        const StoredObjectType      obj     = this->GetById(uObjectId);
        const ::B33::Math::iVec3    area    = ::B33::Math::iVec3::ToiVec3(obj.GetHalfSize() + 1);

        this->RemoveFromGrid(::B33::Math::iVec3::ToiVec3(obj.GetPosition()), 
                             area,
                             uObjectId);
    
        for (auto itObj = m_StoredObjects.begin(); itObj != m_StoredObjects.end(); ++itObj) {
            if (&(*itObj) == &obj) {
                m_StoredObjects.erase(itObj);
                break;
            }
        }
    }

    void UpdatePos(const ::B33::Math::Vec3& newPos, size_t uObjectId)
    {
        StoredObjectType& obj = m_StoredObjects[uObjectId];

        if (m_uObjectsCount >= m_StoredObjects.size() - 1) {
            AB_LOG(Core::Debug::Warning, L"Reached object limit of objects in the world");
            return;
        }

        if (obj.GetPosition() == newPos)
            return;
    
        const ::B33::Math::iVec3 area = ::B33::Math::iVec3::ToiVec3(obj.GetHalfSize() + 1);
        this->RemoveFromGrid(::B33::Math::iVec3::ToiVec3(obj.GetPosition()), 
                             area,
                             uObjectId);

        obj.SetPositon(newPos);
        this->PlaceOnGrid(::B33::Math::iVec3::ToiVec3(newPos), 
                          area,
                          uObjectId);

        this->ForceUpload();
    }

    void UpdateRot(const ::B33::Math::Rot3& newRot, size_t uId)
    {
        if (m_uObjectsCount >= m_StoredObjects.size() - 1) {
            AB_LOG(Core::Debug::Warning, L"Reached object limit of objects in the world");
            return;
        }

        if (m_StoredObjects[uId].GetRotation() == newRot)
			return;

        m_StoredObjects[uId].SetRotation(newRot);

        this->ForceUpload();
    }

private:

    template<class U>
    ::size_t GenerateObject(::B33::Math::iVec3 pos, ::std::vector<Voxel>& voxelsGrid, U&& sot)
    { 
        const size_t uObjId = m_uObjectsCount;
        
        if (m_uObjectsCount >= m_StoredObjects.size() - 1) {
            AB_LOG(Core::Debug::Warning, L"Reached object limit of objects in the world");
            return -1;
        }

        sot.SetPositon(::B33::Math::Vec3::ToVec3(pos));

        this->PlaceOnGrid(::B33::Math::iVec3::ToiVec3(sot.GetPosition()), 
                          ::B33::Math::iVec3::ToiVec3(sot.GetHalfSize() + 1),
                          uObjId);
        
        m_StoredObjects[m_uObjectsCount++] = ::std::forward<U>(sot);
        return uObjId;
    }

private:

    ::std::vector<StoredObjectType> m_StoredObjects;
    ::size_t                        m_uObjectsCount = -1;

};

typedef ::B33::Rendering::WorldGrid<ColoredCube> CubeWorld;

} // !B33::Rendering
#endif // !AB_VOXEL_GRID_H
