#ifndef AB_VEC3_H
#define AB_VEC3_H

#include "B33Core.h"

namespace B33::Math
{

// ---------------------------------------------------------------------------------------------------------------------
struct alignas(16) Vec3
{

    static constexpr size_t Size = 3;
    
public:

    Vec3()
        : x(0.f), y(0.f), z(0.f)
    { } 

    explicit Vec3(float x, float y = 0., float z = 0.f)
        : x(x), y(y), z(z)
    { }

    ~Vec3() = default;

public:

    Vec3(Vec3&&) noexcept = default;
    Vec3(const Vec3&) = default;

    Vec3& operator=(const Vec3&) noexcept = default;
    Vec3& operator=(Vec3&&) noexcept = default;

public:

    float x;
    float y;
    float z;
    float _Padding;

public:

    template<typename Vector>
    static Vec3 ToVec(Vector v)
    { return Vec3(v.x, v.y, v.z); }

public:

    constexpr float& operator[](size_t uIndex);

    constexpr float operator[](size_t uIndex) const;

    inline bool operator==(const Vec3& vB) const;

    inline Vec3& operator+=(const Vec3& vB);

    inline Vec3 operator+(const Vec3& vB) const;

    inline Vec3 operator+(const struct iVec3& vB) const;

    inline Vec3 operator+(const uint32_t vB) const;

    inline Vec3 operator*(const Vec3& vB) const;

    inline Vec3 operator*(const float vB) const;
};

// ---------------------------------------------------------------------------------------------------------------------
struct alignas(16) iVec3
{

    static constexpr size_t Size = 3;

public:

    iVec3()
        : x(0), y(0), z(0)
    { }

    explicit iVec3(int32_t x, int32_t y = 0, int32_t z = 0)
        : x(x), y(y), z(z)
    { }

    explicit iVec3(Vec3 v)
        : x(v.x), y(v.y), z(v.z)
    { }

    ~iVec3() = default;

public:

    iVec3(iVec3&&) noexcept = default;
    iVec3(const iVec3&) = default;

    iVec3& operator=(const iVec3&) noexcept = default;
    iVec3& operator=(iVec3&&) noexcept = default;

public:

    int32_t x;
    int32_t y;
    int32_t z;
    int32_t _Padding;

public:

    template<typename Vector>
    static iVec3 ToVec(Vector v)
    { return iVec3(v.x, v.y, v.z); }
    
public:

    constexpr bool operator==(const iVec3& other) const noexcept 
    {
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }

    constexpr int32_t operator[](size_t uIndex) const
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

    constexpr int32_t& operator[](size_t uIndex)
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

    inline iVec3 operator+(const Vec3& vB) const;

    inline iVec3 operator*(const uint32_t vB) const;
};

} // !B33::Math
#endif // !AB_VEC3_H
