#ifndef AB_RAYS_H
#define AB_RAYS_H

#include "Raycaster/VoxelGrid.hpp"

namespace B33::Rendering
{

struct HitResult 
{
    using Vec   = ::B33::Math::Vec3;
    using iVec  = ::B33::Math::iVec3;

    bool        bHit;
    iVec        iHitCoords;
    ::size_t    uHitIndex;
    float       fDistance;
    Vec         Normal;
};

BEAST_API HitResult MarchTheRay(const ::B33::Rendering::IWorldGrid* worldGrid, 
                                const ::B33::Math::Vec3& rayOrigin,
                                const ::B33::Math::Vec3& rayDestination,
                                ::size_t maxSteps);
     
} //!B33::Rendering
#endif //!AB_RAYS_H
