#ifndef B33_DEBUG_ASSERT_H
#define B33_DEBUG_ASSERT_H

#include "Exception.hpp"

#ifdef _DEBUG
#    define B33_ASSERT( expr )                                                                                         \
        do                                                                                                             \
        {                                                                                                              \
            if ( !( expr ) )                                                                                           \
            {                                                                                                          \
                throw ::B33::Core::Exception( "Assertion failed! ('" #expr "').", __LINE__, __FILE__ );                \
            }                                                                                                          \
        } while ( 0 )

#    define B33_ASSERT_MSG( expr, msg )                                                                                \
        do                                                                                                             \
        {                                                                                                              \
            if ( !( expr ) )                                                                                           \
            {                                                                                                          \
                throw ::B33::Core::Exception( "Assertion failed! ('" #expr "') " msg, __LINE__, __FILE__ );            \
            }                                                                                                          \
        } while ( 0 )
#else
#    define B33_ASSERT( expr )
#    define B33_ASSERT_MSG( expr, msg )
#endif

namespace B33::Core
{

template <typename T>
constexpr bool TypeIsAlwaysFalse = false;

} // namespace B33::Core
#endif // !B33_DEBUG_ASSERT_H
