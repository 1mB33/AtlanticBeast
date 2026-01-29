#ifndef AB_GAME_SYSTEM_POLICY_H
#define AB_GAME_SYSTEM_POLICY_H

#if _WIN32
#	include "Window/WindowPolicy/Win32/WindowModeGameWin32Policy.hpp"
#	include "Window/WindowPolicy/Win32/BorderlessGameWin32Policy.hpp"
#elif __linux__
#   include "Window/WindowPolicy/Linux/GameLinuxPolicy.hpp"
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

} // !B33::App
#endif // !AB_GAME_SYSTEM_POLICY_H

