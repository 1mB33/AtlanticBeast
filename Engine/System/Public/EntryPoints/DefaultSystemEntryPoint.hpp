#ifndef B33_DEFAULT_SYSTEM_ENTRY_POINT_HPP
#define B33_DEFAULT_SYSTEM_ENTRY_POINT_HPP

#if defined( _WIN32 )
#    include "Win32/Win32Entry.hpp"
#elif defined( __linux__ )
#    include "Linux/LinuxEntry.hpp"
#endif

#endif // !B33_DEFAULT_SYSTEM_ENTRY_POINT_HPP
