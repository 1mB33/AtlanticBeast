#ifdef _WIN32
#ifndef AB_GAME_POLICY_H
#define AB_GAME_POLICY_H

#include "Window/WindowPolicy/Win32/BasicWin32Policy.hpp"

namespace B33::App
{

/**
 * @brief Game version of BasicWin32WindowPolicy, hides cursor, captures it and outputs raw deltas from the mouse
 * for the InputStruct.
 */
class BEAST_API WindowModeGameWin32WindowPolicy : public BasicWin32WindowPolicy
{
public:

    void OnPreWcex() override;

    void OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

};

} // !B33::App
#endif // !AB_GAME_POLICY_H
#endif // _WIN32
