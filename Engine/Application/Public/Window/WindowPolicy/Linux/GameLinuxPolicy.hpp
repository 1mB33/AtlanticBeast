#ifdef __linux__
#    ifndef B33_GAME_WINDOW_POLICY_H
#        define B33_GAME_WINDOW_POLICY_H

#        include "Window/WindowPolicy/Linux/BasicLinuxPolicy.hpp"

namespace B33::App
{

/**
 * @brief Game version of BasicLinuxWindowPolicy, hides cursor, captures it and outputs raw deltas from the mouse
 * for the InputStruct.
 */
class BEAST_API GameLinuxWindowPolicy : public BasicLinuxWindowPolicy
{
  public:
    virtual void OnCreate( WindowDesc *pWd ) override;

    virtual uint32_t OnUpdate( WindowDesc *pWd, XEvent &event ) override;

  private:
    void HandleRawInput( WindowDesc *pWd, XEvent &event );

    void HandleFocusIn( WindowDesc *pWd );

    void HandleFocusOut( WindowDesc *pWd );

  private:
    int m_OpCode;
};

} // namespace B33::App
#    endif // !B33_GAME_WINDOW_POLICY_H
#endif     // __linux__
