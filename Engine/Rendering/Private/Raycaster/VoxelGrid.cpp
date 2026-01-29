#include "B33Rendering.hpp"

#include "Raycaster/VoxelGrid.hpp"

namespace B33::Rendering
{

using namespace ::std;
using namespace ::B33::Math;

// --------------------------------------------------------------------------------------------------------------------
void IWorldGrid::SetVoxel(const iVec& pos, uint32_t uColor)
{
    vector<Voxel>& voxelsGrid = this->GetGrid();
    const size_t uDim   = this->GetGridWidth(); 
    const size_t uIndex = CalcIndex(pos);

    AB_ASSERT(uIndex < voxelsGrid.size());

    voxelsGrid[uIndex].Type     = Voxel::FullSolid;
    voxelsGrid[uIndex].Color    = uColor;
    this->ForceUpload();
}

// --------------------------------------------------------------------------------------------------------------------
size_t IWorldGrid::CalcIndex(const iVec& pos) const
{
    return pos.x + 
           pos.y * m_uGridDim +
           pos.z * m_uGridDim * m_uGridDim;
}

// --------------------------------------------------------------------------------------------------------------------
void IWorldGrid::PlaceOnGrid(const iVec& pos, const iVec& area,const size_t uId)
{
    // Incremeant the type on connected voxels'
    size_t uCornerIndex;
    for (int32_t x = -area.x; x <= area.x; ++x) {
        for (int32_t y = -area.y; y <= area.y; ++y) {
            for (int32_t z = -area.z; z <= area.z; ++z)
            {
                uCornerIndex = CalcIndex(iVec(pos.x + x, pos.y + y, pos.z + z));
                
                if (uCornerIndex >= m_VoxelGrid.size()) 
                    continue;

                Voxel& voxel = m_VoxelGrid[uCornerIndex];

                if (voxel.Type == Voxel::FullSolid)
                    continue;

                if (voxel.Type >= Voxel::MaxPerInstance) {
                    AB_LOG(Core::Debug::Warning, L"Reached object limit for the connected voxel");
                    continue;
                }

                voxel.Id[voxel.Type++] = uId;
            }
        }
    }
}

// --------------------------------------------------------------------------------------------------------------------
void IWorldGrid::RemoveFromGrid(const iVec& pos, const iVec& area, const size_t uId)
{
    // Decremeant the type on connected voxels
    size_t uCornerIndex;
    size_t uLastIdOnList;
    size_t uIndexOfIdOnList;
    for (int32_t x = -area.x; x <= area.x; ++x) {
        for (int32_t y = -area.y; y <= area.y; ++y) {
            for (int32_t z = -area.z; z <= area.z; ++z)
            {
                uCornerIndex = CalcIndex(iVec(pos.x + x, pos.y + y, pos.z + z));
                
                if (uCornerIndex >= m_VoxelGrid.size()) 
                    continue;

                Voxel& voxel = m_VoxelGrid[uCornerIndex];

                if (voxel.Type == Voxel::FullSolid || voxel.Type == 0)
                    continue;

                uLastIdOnList = voxel.Id[voxel.Type - 1];

                for (uIndexOfIdOnList = 0; uIndexOfIdOnList < voxel.Type; ++uIndexOfIdOnList)
                    if (voxel.Id[uIndexOfIdOnList] == uId) 
                        break;
        
                voxel.Id[uIndexOfIdOnList] = voxel.Id[--voxel.Type];
            }
        }
    }
}

} //!B33::Rendering
