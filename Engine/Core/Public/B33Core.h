#ifndef AB_CORE_H
#define AB_CORE_H

#include "B33CoreMinimal.h"

#ifdef __cplusplus

#include "Exception.hpp"

namespace B33::Core::Debug
{

static const char szLogPostfix[] = "_AtlanticBeast.log";

} // !B33::Core

#undef AB_EXCEPT
#define AB_EXCEPT(message) ::B33::Core::Exception(message, __LINE__, __FILE__)

#include "Debug/Assert.hpp"
#include "Debug/Logger.hpp"

#ifdef _DEBUG
#   define AB_LOG(...) ::B33::Core::Debug::Logger::Get().Log(__VA_ARGS__)
#else
#   define AB_LOG(...) 
#endif // !_DEBUG

// Workaround for empty __VA_ARGS__
#ifdef _WIN32
#   define AB_VA_ARGS_(...) , __VA_ARGS__
#else
#   define AB_VA_ARGS_(...) __VA_OPT__(,) __VA_ARGS__
#endif // _WIN32

#endif // !__cplusplus
#endif // !AB_CORE_H
