#ifndef AB_CUBE_H
#define AB_CUBE_H

#include "Math.hpp"
#include "Primitives/Object.hpp"

namespace B33::Math
{

class alignas(16) Cube : public WorldObject
{
public:

    Cube() 
        : WorldObject()
        , m_fHalfSize(.5f, .5f, .5f)
    { }

public:

    Vec3 GetHalfSize() const
    { return m_fHalfSize; }

public:

    void SetHalfSize(Vec3 halfSize)
    { m_fHalfSize = halfSize; }

private:

    Vec3 m_fHalfSize;

};

} // !B33::Math
#endif // !AB_CUBE_H
