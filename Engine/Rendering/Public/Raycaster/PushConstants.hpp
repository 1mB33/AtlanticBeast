#ifndef AB_PUSH_CONSTANTS_H
#define AB_PUSH_CONSTANTS_H

#include "B33Rendering.hpp"

namespace B33::Rendering
{

struct alignas(16) VoxelPushConstants
{
    using Vec   = ::B33::Math::Vec3;
    using iVec  = ::B33::Math::iVec3;

    Vec         CameraPos;
    iVec        GridSize;
    Vec         CameraLookDir;
    Vec         CameraRight;
    Vec         CameraUp;
    float       fFov;
    ::uint32_t  uMode;
    ::uint32_t  _Padding2;
    ::uint32_t  _Padding3;
};

} // !B33::Rendering
#endif // !AB_PUSH_CONSTANTS_H
