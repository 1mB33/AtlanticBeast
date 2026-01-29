#include "B33Rendering.hpp"

#include "Raycaster/Rays.hpp"

namespace B33::Rendering
{

using namespace ::std;
using namespace ::B33::Math;

HitResult MarchTheRay(const IWorldGrid* vg,
                      const Vec3& ro,
                      const Vec3& rd,
                      size_t maxSteps)
{
    HitResult result;
    result.bHit         = false;
    result.fDistance    = 0.0f;

    iVec3 voxel(ro.x, ro.y, ro.z);
    iVec3 step(copysign(1.0f, rd.x), copysign(1.0f, rd.y), copysign(1.0f, rd.z));

    Vec3 invDir(1.0f / rd.x, 1.0f / rd.y, 1.0f / rd.z);
    Vec3 tDelta(fabs(invDir.x), fabs(invDir.y), fabs(invDir.z));
    Vec3 tMax;

    for (size_t i = 0; i < 3; ++i)
    {
        float offset = rd[i] > 0.0f ?
                       1.0f - ro[i] + static_cast<uint32_t>(ro[i]) : 
                       ro[i] - static_cast<uint32_t>(ro[i]);

        tMax[i] = tDelta[i] * offset;
    }

    enum LastStepAxis {
        Unknown = 1,
        X = Unknown << 1,
        Y = X << 1,
        Z = Y << 1,
    };
    LastStepAxis lastStepAxis = Unknown;
    const size_t gridWidth = vg->GetGridWidth();

    for (size_t stepCount = 0; stepCount < maxSteps; ++stepCount)
    {
        if (voxel.x < 0 || voxel.x >= gridWidth ||
            voxel.y < 0 || voxel.y >= gridWidth ||
            voxel.z < 0 || voxel.z >= gridWidth)
        {
            break;
        }

        const size_t index = voxel.x +
                             voxel.y * gridWidth +
                             voxel.z * gridWidth * gridWidth;

        if (vg->GetGrid()[index].Type == static_cast<uint32_t>(-1) ||
            (vg->GetGrid()[index].Type > 0 && vg->CheckIfVoxelOccupied(voxel)))
        {
            result.bHit         = true;
            result.iHitCoords   = voxel;
            result.uHitIndex    = index;

            switch (lastStepAxis) {
                case X:
                    result.Normal       = Vec3(-float(step.x), 0.0f, 0.0f);
                    result.fDistance    = tMax.x - tDelta.x;
                    break;
                case Y:
                    result.Normal       = Vec3(0.0f, -float(step.y), 0.0f);
                    result.fDistance    = tMax.y - tDelta.y;
                    break;
                case Z:
                    result.Normal       = Vec3(0.0f, 0.0f, -float(step.z));
                    result.fDistance    = tMax.z - tDelta.z;
                    break;
                default:
                    result.Normal       = Vec3(0.f, 0.f, 0.f);
                    result.fDistance    = tMax.z - tDelta.z;
            }

            return result;
        }

        if (tMax.x < tMax.y)
        {
            if (tMax.x < tMax.z) {
                voxel.x += step.x;
                tMax.x  += tDelta.x;
                lastStepAxis = X;
            }
            else {
                voxel.z += step.z;
                tMax.z  += tDelta.z;
                lastStepAxis = Z;
            }
        }
        else
        {
            if (tMax.y < tMax.z) {
                voxel.y += step.y;
                tMax.y  += tDelta.y;
                lastStepAxis = Y;
            }
            else {
                voxel.z += step.z;
                tMax.z  += tDelta.z;
                lastStepAxis = Z;
            }
        }
    }

    return result;
}

} // !Voxels
