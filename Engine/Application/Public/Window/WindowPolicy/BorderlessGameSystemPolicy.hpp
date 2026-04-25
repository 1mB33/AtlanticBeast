#ifndef B33_BORDERLESS_GAME_SYSTEM_POLICY_H
#define B33_BORDERLESS_GAME_SYSTEM_POLICY_H

#if defined( _WIN32 )
#    include "Window/WindowPolicy/Win32/BorderlessGameWin32Policy.hpp"
#elif defined( _X11 )
#    include "Window/WindowPolicy/Linux/BorderlessGameLinuxPolicy.hpp"
#elif defined( __APPLE__ )
#    include "Window/WindowPolicy/Apple/BasicApplePolicy.hpp"
#endif // !_WIN32

namespace B33::App
{

typedef
#if defined( _WIN32 )
    BorderlessGameWin32Policy
#elif defined( _X11 )
    BorderlessGameLinuxWindowPolicy
#elif defined( __APPLE__ )
    BasicAppleWindowPolicy
#endif // !_WIN32
        BorderlessGameSystemPolicy;

} // namespace B33::App
#endif // !B33_BORDERLESS_GAME_SYSTEM_POLICY_H
