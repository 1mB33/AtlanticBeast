#ifndef B33_EDITOR_PUSH_CONSTANTS_H
#define B33_EDITOR_PUSH_CONSTANTS_H

#include "Vulkan/IPushConstants.hpp"

namespace B33::Rendering
{

struct alignas( 16 ) EditorPushConstants : IPushConstants
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

    Vec        CameraPos;
    iVec       GridSize;
    Vec        CameraLookDir;
    Vec        CameraRight;
    Vec        CameraUp;
    float      fFov;
    ::uint32_t uMode;
    ::uint32_t _Padding2;
    ::uint32_t _Padding3;
};

} // namespace B33::Rendering
#endif // !B33_EDITOR_PUSH_CONSTANTS_H
