#ifndef AB_VOXEL_H
#define AB_VOXEL_H

#include "B33Rendering.hpp"

namespace B33::Rendering
{

struct alignas(16) Voxel
{
    constexpr const static ::uint32_t MaxPerInstance = 26;
    constexpr const static ::uint32_t FullSolid = -1;

    ::uint32_t Type;
    ::uint32_t Color;
    ::uint32_t Id[MaxPerInstance];
};

} // !Voxels
#endif // !AB_VOXEL_H
