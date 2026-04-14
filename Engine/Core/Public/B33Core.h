#ifndef B33_CORE_H
#define B33_CORE_H

#include "B33CoreMinimal.h"

#ifdef __cplusplus

#    include "Exception.hpp"

namespace B33::Core::Debug
{

static const char szLogPostfix[] = "_AtlanticBeast.log";

} // namespace B33::Core::Debug

#    undef B33_EXCEPT
#    define B33_EXCEPT( message ) ::B33::Core::Exception( message, __LINE__, __FILE__ )

#    include "Debug/Assert.hpp"
#    include "Debug/Logger.hpp"

#    ifdef _DEBUG
#        define B33_LOG( ... )     ::B33::Core::Debug::Logger::Get().Log( __VA_ARGS__ )
#        define B33_INFO( ... )    ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, __VA_ARGS__ )
#        define B33_TRACE( ... )   ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Trace, __VA_ARGS__ )
#        define B33_WARNING( ... ) ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Warning, __VA_ARGS__ )
#        define B33_ERROR( ... )   ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Error, __VA_ARGS__ )
// #        define B33_TRACE( ... )
#    else
#        define B33_LOG( ... )
#        define B33_INFO( ... )
#        define B33_TRACE( ... )
#        define B33_WARNING( ... )
#        define B33_ERROR( ... )
#    endif // !_DEBUG

// Workaround for empty __VA_ARGS__
#    ifdef _WIN32
#        define B33_VA_ARGS_( ... ) , __VA_ARGS__
#    else
#        define B33_VA_ARGS_( ... ) __VA_OPT__(, ) __VA_ARGS__
#    endif // _WIN32

#endif // !__cplusplus
#endif // !B33_CORE_H
