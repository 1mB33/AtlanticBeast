#if defined( X11_FOUND )
#    ifndef B33_BORDERLESS_GAME_WINDOW_POLICY_H
#        define B33_BORDERLESS_GAME_WINDOW_POLICY_H

#        include "Window/WindowPolicy/Linux/GameLinuxPolicy.hpp"

namespace B33::App
{

/**
 * @brief Game version of BasicLinuxWindowPolicy, hides cursor, captures it and outputs raw deltas from the mouse
 * for the InputStruct.
 */
class BEAST_API BorderlessGameLinuxWindowPolicy : public GameLinuxWindowPolicy
{
  public:
    virtual void OnCreate( WindowDesc *pWd ) override;

  private:
    Atom m_WMState;
    Atom m_WMFullscreen;
};

} // namespace B33::App
#    endif // !B33_BORDERLESS_GAME_WINDOW_POLICY_H
#endif     // __linux__
