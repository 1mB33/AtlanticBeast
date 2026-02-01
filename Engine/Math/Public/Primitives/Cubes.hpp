#ifndef AB_CUBES_H
#define AB_CUBES_H

#include "B33Math.hpp"
#include "Primitives/Objects.hpp"

namespace B33::Math
{

class alignas(16) Cubes : public WorldObjects
{
public:

    Cubes() 
        : WorldObjects()
        , m_fHalfSizes(64 * 64 * 64)
    { }

public:

    const ::std::vector<Vec3>& GetHalfSizes() const
    { return m_fHalfSizes; }

    Vec3 GetHalfSize(::size_t uIndex) const
    { return m_fHalfSizes[uIndex]; }

public:

    void SetHalfSize(Vec3 halfSize, ::size_t uIndex)
    { m_fHalfSizes[uIndex] = halfSize; }

public:

    virtual ::size_t AddObject() override 
    { return WorldObjects::AddObject(); }

    virtual void RemoveObject(::size_t uIndex) override
    { WorldObjects::RemoveObject(uIndex); }

private:

    ::std::vector<Vec3> m_fHalfSizes;

};

} // !B33::Math
#endif // !AB_CUBES_H
