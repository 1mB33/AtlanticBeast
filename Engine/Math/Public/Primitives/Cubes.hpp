#ifndef AB_CUBES_H
#define AB_CUBES_H

#include "B33Math.hpp"
#include "Primitives/Objects.hpp"

namespace B33::Math
{

class Cubes : public WorldObjects
{
public:

    Cubes() 
        : WorldObjects()
        , m_vHalfSizes({ })
    { 
        m_vHalfSizes.reserve(64 * 64 * 64);
    }

public:

    const ::std::vector<Vec3>& GetHalfSizes() const
    { return m_vHalfSizes; }

    Vec3 GetHalfSize(::size_t uIndex) const
    { return m_vHalfSizes[uIndex]; }

public:

    void SetHalfSize(Vec3 halfSize, ::size_t uIndex)
    { m_vHalfSizes[uIndex] = halfSize; }

public:

    virtual ::size_t AddObject() override 
    { 
        ::size_t i = WorldObjects::AddObject(); 
        m_vHalfSizes.push_back(Vec3());

        AB_ASSERT(i == m_vHalfSizes.size() - 1);

        return i;
    }

    virtual void RemoveObject(::size_t uIndex) override
    { WorldObjects::RemoveObject(uIndex); }

private:

    ::std::vector<Vec3> m_vHalfSizes;

};

} // !B33::Math
#endif // !AB_CUBES_H
