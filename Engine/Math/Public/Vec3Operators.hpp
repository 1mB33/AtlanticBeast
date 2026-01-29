#ifndef AB_VEC3_OPERATORS_H
#define AB_VEC3_OPERATORS_H

#include "Vec3.hpp"
#include "Operations.hpp"

namespace B33::Math
{
    
// --------------------------------------------------------------------------------------------------------------------
constexpr float& Vec3::operator[](size_t uIndex)
{
    AB_ASSERT(uIndex < Size);

    if (uIndex == 0) {
        return x;
    }
    if (uIndex == 1) {
        return y;
    }
    return z;
} 

// --------------------------------------------------------------------------------------------------------------------
constexpr float Vec3::operator[](size_t uIndex) const
{
    AB_ASSERT(uIndex < Size);

    if (uIndex == 0) {
        return x;
    }
    if (uIndex == 1) {
        return y;
    }
    return z;
}

// --------------------------------------------------------------------------------------------------------------------
inline bool Vec3::operator==(const Vec3& vB) const
{
	const float fEpsilon = 0.0001f;
    return (::std::fabs(this->x - vB.x) < fEpsilon &&
            ::std::fabs(this->y - vB.y) < fEpsilon &&
            ::std::fabs(this->z - vB.z) < fEpsilon) ? true : false;
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3& Vec3::operator+=(const Vec3& vB)
{
    return AddAssign(*this, vB);
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator+(const Vec3& vB) const
{
    Vec3 n(*this);
    return AddAssign(n, vB);
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator+(const iVec3& vB) const
{
    Vec3 n(*this);
    n = AddAssign(n, Vec3::ToVec(vB));
    return n;
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator+(const uint32_t vB) const
{
    Vec3 n(*this);
    n = AddAssign(n, Vec3(vB, vB, vB));
    return n;
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator*(const Vec3& vB) const
{
    Vec3 n(*this);
    return Multiply(n, vB);
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator*(const float vB) const
{
    Vec3 n(*this);
    return MultiplyScalar(n, vB);
}

// --------------------------------------------------------------------------------------------------------------------
inline iVec3 iVec3::operator+(const Vec3& vB) const
{
    iVec3 n(*this);
    return AddAssign(n, iVec3(vB));
}

// --------------------------------------------------------------------------------------------------------------------
inline iVec3 iVec3::operator*(const uint32_t vB) const
{
    iVec3 n(*this);
    return Multiply(n, iVec3(vB, vB, vB));
}

} // !B33::Math
#endif // !AB_VEC3_OPERATORS_H
