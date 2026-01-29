#ifndef AB_VEC4_H
#define AB_VEC4_H

#include "B33Core.h"

namespace B33::Math
{

// ---------------------------------------------------------------------------------------------------------------------
struct alignas(16) Vec4
{ 

    static constexpr size_t Size = 4;

public:

    explicit Vec4(float x = 0, float y = 0, float z = 0, float w = 0)
        : x(x), y(y), z(z), w(w)
    { }

    ~Vec4() = default;

public:

    Vec4(Vec4&&) noexcept = default;
    Vec4(const Vec4&) = default;

    Vec4& operator=(const Vec4&) noexcept = default;
    Vec4& operator=(Vec4&&) noexcept = default;

public:

    float x;
    float y;
    float z;
    float w;

public:

    template<typename Vector>
    static Vec4 ToVec4(Vector v)
    { return Vec4(v.x, v.y, v.z, 0.f); }

public:

    constexpr float operator[](size_t uIndex) const
    {
        AB_ASSERT(uIndex < Size);

        if (uIndex == 0) {
            return x;
        }
        if (uIndex == 1) {
            return y;
        }
        if (uIndex == 2) {
            return z;
        }
        return w;
    } 

    constexpr float& operator[](size_t uIndex)
    {
        AB_ASSERT(uIndex < Size);

        if (uIndex == 0) {
            return x;
        }
        if (uIndex == 1) {
            return y;
        }
        if (uIndex == 2) {
            return z;
        }
        return w;
    }
};

// ---------------------------------------------------------------------------------------------------------------------
struct alignas(16) iVec4
{ 

    static constexpr size_t Size = 4;

public:

    explicit iVec4(int32_t x = 0, int32_t y = 0, int32_t z = 0, int32_t w = 0)
        : x(x), y(y), z(z), w(w)
    { }

    ~iVec4() = default;

public:

    iVec4(iVec4&&) noexcept = default;
    iVec4(const iVec4&) = default;

    iVec4& operator=(const iVec4&) noexcept = default;
    iVec4& operator=(iVec4&&) noexcept = default;

public:

    int32_t x;
    int32_t y;
    int32_t z;
    int32_t w;

public:

    template<typename Vector>
    static iVec4 ToiVec4(Vector v)
    { return iVec4(v.x, v.y, v.z, 0.f); }

public:
    
    constexpr int32_t operator[](size_t uIndex) const
    {
        AB_ASSERT(uIndex < Size);

        if (uIndex == 0) {
            return x;
        }
        if (uIndex == 1) {
            return y;
        }
        if (uIndex == 2) {
            return z;
        }
        return w;
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
        if (uIndex == 2) {
            return z;
        }
        return w;
    }
};

} // !B33::Math
#endif // !AB_VEC4_H
