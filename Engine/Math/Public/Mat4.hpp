#ifndef B33_MAT4_H
#define B33_MAT4_H

namespace B33::Math
{

struct alignas( 16 ) Mat4
{
    float m[ 16 ];
};

} // namespace B33::Math
#endif // !B33_MAT4_H
