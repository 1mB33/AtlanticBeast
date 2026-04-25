#ifndef B33_GAME_SYSTEM_POLICY_H
#define B33_GAME_SYSTEM_POLICY_H

#include "B33App.h"

#if defined( _WIN32 )
#    include "Window/WindowPolicy/Win32/BorderlessGameWin32Policy.hpp"
#    include "Window/WindowPolicy/Win32/WindowModeGameWin32Policy.hpp"
#elif defined( __linux__ )
#    include "Window/WindowPolicy/Linux/GameLinuxPolicy.hpp"
#elif defined( __APPLE__ )
#    include "Window/WindowPolicy/Apple/BasicApplePolicy.hpp"
#endif // !_WIN32

namespace B33::App
{

typedef
#if defined( _WIN32 )
    WindowModeGameWin32WindowPolicy
#elif defined( __linux__ )
    GameLinuxWindowPolicy
#elif defined( __APPLE__ )
    BasicAppleWindowPolicy
#endif // !_WIN32
        DefaultGameSystemWindowPolicy;

} // namespace B33::App
#endif // !B33_GAME_SYSTEM_POLICY_H
