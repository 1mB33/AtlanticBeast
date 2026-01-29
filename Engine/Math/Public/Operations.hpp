#ifndef AB_OPERATIONS_H
#define AB_OPERATIONS_H

#include "Vec3.hpp"

namespace B33::Math
{

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
Vector Normalize(const Vector& v)
{
    Vector result = Vector();
    float mod = 0.f;
    float invMag;

    for (size_t i = 0; i < Vector::Size; ++i) 
        mod += v[i] * v[i];

    if (mod == 0.f) {
        return result;
    }

    invMag = 1.f / ::std::sqrt(mod);

    for (size_t i = 0; i < Vector::Size; ++i)
        result[i] = v[i] * invMag;

    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
constexpr inline Vector Cross(const Vector& vA, const Vector& vB)
{
    static_assert(Core::TypeIsAlwaysFalse<Vector>, 
                  "This size of a vector doesn't have impementation of cross product yet");
}

// ---------------------------------------------------------------------------------------------------------------------
template<>
inline Vec3 Cross(const Vec3& vA, const Vec3& vB)
{
    Vec3 result;
    result.x = vA.y * vB.z - vB.y * vA.z;
    result.y = vA.z * vB.x - vB.z * vA.x;
    result.z = vA.x * vB.y - vB.x * vA.y;
    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
constexpr inline Vector RotateX(const Vector& v, float angleRad)
{
    static_assert(Core::TypeIsAlwaysFalse<Vector>, 
                  "This size of a vector doesn't have impementation of rotate x yet");
}

// ---------------------------------------------------------------------------------------------------------------------
template<>
inline Vec3 RotateX(const Vec3& v, float angleRad)
{
    Vec3 result;
    float c = std::cos(angleRad);
    float s = std::sin(angleRad);

    result.x = v.x;
    result.y = v.y * c - v.z * s;
    result.z = v.y * s + v.z * c;

    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
constexpr inline Vector RotateY(const Vector& v, float angleRad)
{
    static_assert(Core::TypeIsAlwaysFalse<Vector>, 
                  "This size of a vector doesn't have impementation of rotate y yet");
}

// ---------------------------------------------------------------------------------------------------------------------
template<>
inline Vec3 RotateY(const Vec3& v, float angleRad)
{
    Vec3 result;
    float c = std::cos(angleRad);
    float s = std::sin(angleRad);

    result.x = v.x * c + v.z * s;
    result.y = v.y;
    result.z = -v.x * s + v.z * c;

    return result;
}
// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
constexpr inline Vector RotateZ(const Vector& v, float angleRad)
{
    static_assert(Core::TypeIsAlwaysFalse<Vector>, 
                  "This size of a vector doesn't have impementation of rotate z yet");
}

// ---------------------------------------------------------------------------------------------------------------------
template<>
inline Vec3 RotateZ(const Vec3& v, float angleRad)
{
    Vec3 result;
    float c = std::cos(angleRad);
    float s = std::sin(angleRad);

    result.x = v.x * c - v.y * s;
    result.y = v.x * s + v.y * c;
    result.z = v.z;

    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
inline Vector& AddAssign(Vector& vA, const Vector& vB)
{
    static_assert(Core::TypeIsAlwaysFalse<Vector>, 
                  "This size of a vector doesn't have impementation of add assign yet");
}

// ---------------------------------------------------------------------------------------------------------------------
template<>
inline Vec3& AddAssign(Vec3& vA, const Vec3& vB)
{
    vA.x += vB.x;
    vA.y += vB.y;
    vA.z += vB.z;
    return vA;
}

// ---------------------------------------------------------------------------------------------------------------------
template<>
inline iVec3& AddAssign(iVec3& vA, const iVec3& vB)
{
    vA.x += vB.x;
    vA.y += vB.y;
    vA.z += vB.z;
    return vA;
}

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
inline Vector Multiply(const Vector& vA, const Vector& vB)
{
    static_assert(Core::TypeIsAlwaysFalse<Vector>, 
                  "This size of a vector doesn't have impementation of multiply yet");
}

// ---------------------------------------------------------------------------------------------------------------------
template<>
inline Vec3 Multiply(const Vec3& vA, const Vec3& vB)
{
    Vec3 r;
    
    r.x = vA.x * vB.x;
    r.y = vA.y * vB.y;
    r.z = vA.z * vB.z;
    
    return r;
}

// ---------------------------------------------------------------------------------------------------------------------
template<>
inline iVec3 Multiply(const iVec3& vA, const iVec3& vB)
{
    iVec3 r;
    
    r.x = vA.x * vB.x;
    r.y = vA.y * vB.y;
    r.z = vA.z * vB.z;
    
    return r;
}

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
constexpr inline Vector MultiplyScalar(const Vector& vA, const float vB)
{
    static_assert(Core::TypeIsAlwaysFalse<Vector>, 
                  "This size of a vector doesn't have impementation of multiply by scalar yet");
}

// --------------------------------------------------------------------------------------------------------------------
template<>
inline Vec3 MultiplyScalar(const Vec3& vA, const float vB)
{
    Vec3 r;

    r.x = vA.x * vB;
    r.y = vA.y * vB;
    r.z = vA.z * vB;

    return r;
}
// --------------------------------------------------------------------------------------------------------------------
template<>
inline iVec3 MultiplyScalar(const iVec3& vA, const float vB)
{
    iVec3 r;

    r.x = vA.x * vB;
    r.y = vA.y * vB;
    r.z = vA.z * vB;

    return r;
}

} // !B33::Math
#endif // !AB_OPERATIONS_H
