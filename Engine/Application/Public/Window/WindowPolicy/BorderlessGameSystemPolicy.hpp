#ifndef B33_BORDERLESS_GAME_SYSTEM_POLICY_H
#define B33_BORDERLESS_GAME_SYSTEM_POLICY_H

#if defined( _WIN32 )
#    include "Window/WindowPolicy/Win32/BorderlessGameWin32Policy.hpp"
#elif defined( X11_FOUND )
#    include "Window/WindowPolicy/Linux/BorderlessGameLinuxPolicy.hpp"
#endif // !_WIN32

namespace B33::App
{

typedef
#if defined( _WIN32 )
    BorderlessGameWin32Policy
#elif defined( X11_FOUND )
    BorderlessGameLinuxWindowPolicy
#endif // !_WIN32
        BorderlessGameSystemPolicy;

} // namespace B33::App
#endif // !B33_BORDERLESS_GAME_SYSTEM_POLICY_H
