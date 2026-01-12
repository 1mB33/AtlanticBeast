#ifndef AB_PUSH_CONSTANTS_H
#define AB_PUSH_CONSTANTS_H

#include "Voxels.hpp"

namespace Voxels
{

struct alignas(16) VoxelPushConstants
{
    Vec3        CameraPos;
    iVec3       GridSize;
    Vec3        CameraLookDir;
    Vec3        CameraRight;
    Vec3        CameraUp;
    float       fFov;
    uint32_t    uMode;
    uint32_t    _Padding2;
    uint32_t    _Padding3;
};

} // !Voxels
#endif // !AB_PUSH_CONSTANTS_H
