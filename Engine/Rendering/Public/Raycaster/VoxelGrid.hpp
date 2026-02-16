#ifndef AB_VOXEL_GRID_H
#define AB_VOXEL_GRID_H

#include "B33Core.h"
#include "Primitives/Object.hpp"
#include "Vulkan/MemoryUploadTracker.hpp"
#include "Raycaster/Voxel.hpp"
#include "Primitives/ColoredCubes.hpp"

namespace B33::Rendering
{

enum EGridChanged
{
    NoChanges = 1,
    Position = NoChanges << 1,
    Rotation = Position << 1,
    HalfSize = Rotation << 1,
};

class IWorldGrid : public ::B33::Rendering::MemoryUploadTracker
{

    using Vec   = ::B33::Math::Vec3;
    using iVec  = ::B33::Math::iVec3;

protected:

    static constexpr size_t DefaultVoxelGridDim = 64;

public:

    explicit IWorldGrid(size_t uGridWidth = DefaultVoxelGridDim)
        : m_uGridDim(uGridWidth)
        , m_VoxelGrid(uGridWidth * uGridWidth * uGridWidth)
        , m_uChanged(NoChanges)
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

    uint32_t GetChanged() 
    {
        uint32_t r = m_uChanged;
        m_uChanged &= 0;
        return r; 
    }

    virtual const ::B33::Math::WorldObjects& GetStoredObjects() const = 0;

public:

    BEAST_API void SetVoxel(const iVec& pos, uint32_t uColor);

public:

    virtual bool CheckIfVoxelOccupied(const iVec& pos) const = 0;

protected:

    BEAST_API ::size_t CalcIndex(const iVec& pos) const;

    BEAST_API void PlaceOnGrid(const iVec& pos,
                               const iVec& area,
                               const ::size_t uId);

    BEAST_API void RemoveFromGrid(const iVec& pos, 
                                  const iVec& area,
                                  const ::size_t uId);

    void SetPositionChanged() 
    { m_uChanged |= EGridChanged::Position; }

    void SetRotationChanged() 
    { m_uChanged |= EGridChanged::Rotation; }

    void SetHalfSizeChanged() 
    { m_uChanged |= EGridChanged::HalfSize; }

private:

    ::size_t                                m_uGridDim      = -1;
    ::std::vector<::B33::Rendering::Voxel>  m_VoxelGrid;
    ::uint32_t                              m_uChanged;

};

template<class StoredObjectType>
class WorldGrid : public IWorldGrid
{

    using Vec   = ::B33::Math::Vec3;
    using iVec  = ::B33::Math::iVec3;
    using Rot   = ::B33::Math::Rot3;

public:

    explicit WorldGrid(size_t uGridWidth = IWorldGrid::DefaultVoxelGridDim)
        : IWorldGrid(uGridWidth)
        , m_StoredObjects() // TODO: this->GetVoxelsSizeInBytes() / sizeof(Voxel))
        , m_uObjectsCount(0)
    { }

public:

    virtual const ::B33::Math::WorldObjects& GetStoredObjects() const override 
    { return m_StoredObjects; }
    
public:

    virtual bool CheckIfVoxelOccupied(const iVec& pos) const override
    { 
        // AB_ASSERT(CalcIndex(pos) < voxelsGrid.size());

        const ::std::vector<Voxel>& voxelsGrid  = this->GetGrid();
        const ::size_t              uDim        = this->GetGridWidth(); 
        const ::size_t              uIndex      = CalcIndex(pos);

        if (voxelsGrid[uIndex].Type == 0)
            return false;

        for (::uint32_t i = 0; i < voxelsGrid[uIndex].Type; ++i)
            if (iVec::ToVec(m_StoredObjects.GetPosition(voxelsGrid[uIndex].Id[i])) == pos) 
                return true;
        
        return false;
    }

public:

    template<class U>
    size_t GenerateObjectAtVoxel(const iVec& pos, U&& sot)
    {
        size_t uId = GenerateObject(pos, this->GetGrid(), ::std::forward<U>(sot));
        this->ForceUpload();
        this->SetPositionChanged();
        this->SetRotationChanged();
        this->SetHalfSizeChanged();
        return uId;
    }

    void RemoveObject(const size_t uObjectId)
    {
        const iVec area = iVec::ToVec(m_StoredObjects.GetHalfSize(uObjectId) + 1);

        this->RemoveFromGrid(iVec::ToVec(m_StoredObjects.GetPosition(uObjectId)), 
                             area,
                             uObjectId);
    
        this->SetPositionChanged();
        this->SetRotationChanged();
        this->SetHalfSizeChanged();
        m_StoredObjects.RemoveObject(uObjectId);
        // for (auto itObj = m_StoredObjects.begin(); itObj != m_StoredObjects.end(); ++itObj) {
        //     if (&(*itObj) == &obj) {
        //         m_StoredObjects.erase(itObj);
        //         break;
        //     }
        // }
    }

    void UpdatePos(const Vec& newPos, size_t uObjectId)
    {
        // if (m_uObjectsCount >= m_StoredObjects.size() - 1) {
        //     AB_LOG(Core::Debug::Warning, L"Reached object limit of objects in the world");
        //     return;
        // }

        if (m_StoredObjects.GetPosition(uObjectId) == newPos)
            return;
    
        const iVec area = iVec::ToVec(m_StoredObjects.GetHalfSize(uObjectId) + 1);
        this->RemoveFromGrid(iVec::ToVec(m_StoredObjects.GetPosition(uObjectId)), 
                             area,
                             uObjectId);

        m_StoredObjects.SetPositon(newPos, uObjectId);
        this->PlaceOnGrid(iVec::ToVec(newPos), 
                          area,
                          uObjectId);

        this->ForceUpload();
        this->SetPositionChanged();
    }

    void UpdateRot(const Rot& newRot, ::size_t uId)
    {
        // if (m_uObjectsCount >= m_StoredObjects.size() - 1) {
        //     AB_LOG(Core::Debug::Warning, L"Reached object limit of objects in the world");
        //     return;
        // }

        if (m_StoredObjects.GetRotation(uId) == newRot) {
            // AB_LOG(::B33::Core::Debug::Info, L"New rotation is equal to old one");
			return;
        }

        m_StoredObjects.SetRotation(newRot, uId);

        this->ForceUpload();
        this->SetRotationChanged();
    }

private:

    template<class U>
    ::size_t GenerateObject(iVec pos, ::std::vector<Voxel>& voxelsGrid, U&& sot)
    { 
        const size_t uObjId = m_StoredObjects.AddObject();
        
        // if (m_uObjectsCount >= m_StoredObjects.size() - 1) {
        //     AB_LOG(Core::Debug::Warning, L"Reached object limit of objects in the world");
        //     return -1;
        // }

        m_StoredObjects.SetPositon(Vec::ToVec(pos), uObjId);
        m_StoredObjects.SetRotation(sot.GetRotation(), uObjId);
        m_StoredObjects.SetHalfSize(sot.GetHalfSize(), uObjId);

        this->PlaceOnGrid(iVec::ToVec(m_StoredObjects.GetPosition(uObjId)), 
                          iVec::ToVec(m_StoredObjects.GetHalfSize(uObjId) + 1),
                          uObjId);
        
        // m_StoredObjects[m_uObjectsCount++] = ::std::forward<U>(sot);
        return uObjId;
    }

private:

    StoredObjectType m_StoredObjects;
    ::size_t         m_uObjectsCount = -1;

};

typedef ::B33::Rendering::WorldGrid<ColoredCubes> CubeWorld;

} // !B33::Rendering
#endif // !AB_VOXEL_GRID_H
