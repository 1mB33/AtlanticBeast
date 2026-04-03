#ifndef B33_VOXEL_H
#define B33_VOXEL_H

namespace B33::Rendering
{

struct alignas( 16 ) Voxel
{
    constexpr const static ::uint32_t MaxPerInstance = 26;
    constexpr const static ::uint32_t FullSolid      = -1;

    ::uint32_t Type;
    ::uint32_t Color;
    ::uint32_t Id[ MaxPerInstance ];
};

} // namespace B33::Rendering
#endif // !B33_VOXEL_H
