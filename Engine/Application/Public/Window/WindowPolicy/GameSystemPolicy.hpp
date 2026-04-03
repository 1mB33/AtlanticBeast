#ifndef B33_GAME_SYSTEM_POLICY_H
#define B33_GAME_SYSTEM_POLICY_H

#include "B33App.h"

#if _WIN32
#    include "Window/WindowPolicy/Win32/BorderlessGameWin32Policy.hpp"
#    include "Window/WindowPolicy/Win32/WindowModeGameWin32Policy.hpp"
#elif __linux__
#    include "Window/WindowPolicy/Linux/GameLinuxPolicy.hpp"
#endif // !_WIN32

namespace B33::App
{

typedef
#if _WIN32
    WindowModeGameWin32WindowPolicy
#elif __linux__
    GameLinuxWindowPolicy
#endif // !_WIN32
        DefaultGameSystemWindowPolicy;

} // namespace B33::App
#endif // !B33_GAME_SYSTEM_POLICY_H
