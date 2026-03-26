#ifndef AB_BORDERLESS_GAME_SYSTEM_POLICY_H
#define AB_BORDERLESS_GAME_SYSTEM_POLICY_H

#if _WIN32
#    include "Window/WindowPolicy/Win32/BorderlessGameWin32Policy.hpp"
#elif __linux__
#    include "Window/WindowPolicy/Linux/BorderlessGameLinuxPolicy.hpp"
#endif // !_WIN32

namespace B33::App
{

typedef
#if _WIN32
    BorderlessGameWin32Policy
#elif __linux__
    BorderlessGameLinuxWindowPolicy
#endif // !_WIN32
        BorderlessGameSystemPolicy;

} // namespace B33::App
#endif // !AB_BORDERLESS_GAME_SYSTEM_POLICY_H
