#ifndef AB_DEFAULT_SYSTEM_ENTRY_POINT_HPP
#define AB_DEFAULT_SYSTEM_ENTRY_POINT_HPP

#if defined( _WIN32 )
// #include
#elif defined( __linux__ )
#    include "Linux/LinuxEntry.hpp"
#endif

#endif // !AB_DEFAULT_SYSTEM_ENTRY_POINT_HPP
