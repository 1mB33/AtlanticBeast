#ifdef _WIN32
#    ifndef B33_GAME_POLICY_H
#        define B33_GAME_POLICY_H

#        include "Window/WindowPolicy/Win32/BasicWin32Policy.hpp"

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

    void OnUpdate( UINT uMsg, WPARAM wParam, LPARAM lParam ) override;
};

} // namespace B33::App
#    endif // !B33_GAME_POLICY_H
#endif     // _WIN32
