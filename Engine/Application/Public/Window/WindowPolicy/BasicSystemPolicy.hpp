#ifndef B33_BASIC_SYSTEM_POLICY_H
#define B33_BASIC_SYSTEM_POLICY_H

#if _WIN32
#    include "Window/WindowPolicy/Win32/BasicWin32Policy.hpp"
#elif __linux__
#    include "Window/WindowPolicy/Linux/BasicLinuxPolicy.hpp"
#endif // !_WIN32

namespace B33::App
{

typedef
#if _WIN32
    BasicWin32WindowPolicy
#elif __linux__
    BasicLinuxWindowPolicy
#endif // !_WIN32
        DefaultSystemWindowPolicy;

} // namespace B33::App
#endif // !B33_BASIC_SYSTEM_POLICY_H
