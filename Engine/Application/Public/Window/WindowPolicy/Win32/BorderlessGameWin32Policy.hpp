#ifdef _WIN32
#    ifndef B33_BORDERLESS_GAME_WIN32_POLICY_H
#        define B33_BORDERLESS_GAME_WIN32_POLICY_H

#        include "Window/WindowPolicy/Win32/WindowModeGameWin32Policy.hpp"

namespace B33::App
{

/**
 * @brief Borderless fullscreen implementation of WindowModeGameWin32WindowPolicy.
 *
 * On creation, it queries current display resolution and creates a window
 * that covers the whole screen without borders.
 */
class BorderlessGameWin32Policy : public WindowModeGameWin32WindowPolicy
{
  public:
    BEAST_API void OnPreWcex() override;

    BEAST_API void OnPreRegister() override;
};

} // namespace B33::App
#    endif // !B33_BORDERLESS_GAME_WIN32_POLICY_H
#endif     // !_WIN32
