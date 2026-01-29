#ifndef AB_DEBUG_ASSERT_H
#define AB_DEBUG_ASSERT_H

#include "Exception.hpp"

#ifdef _DEBUG
#   define AB_ASSERT(expr)                                                                          \
        do { if (!(expr)) {                                                                         \
            throw ::B33::Core::Exception("Assertion failed! ('" #expr  "').", __LINE__, __FILE__);  \
        } } while (0)                                                               

#   define AB_ASSERT_MSG(expr, msg)                                                                     \
        do { if (!(expr)) {                                                                             \
            throw ::B33::Core::Exception("Assertion failed! ('" #expr  "') " msg, __LINE__, __FILE__);  \
        } } while (0)                                                               
#else
#   define AB_ASSERT(expr)
#   define AB_ASSERT_MSG(expr, msg)
#endif

namespace B33::Core
{

template<typename T>
constexpr bool TypeIsAlwaysFalse = false;

} //!B33::Core
#endif // !AB_DEBUG_ASSERT_H
