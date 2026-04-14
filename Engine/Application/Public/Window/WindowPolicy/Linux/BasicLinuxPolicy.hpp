#if defined( _X11 )
#    ifndef B33_WINDOW_POLICY_H
#        define B33_WINDOW_POLICY_H

#        include "Window/WindowPolicy/IWindowPolicy.hpp"

namespace B33::App
{

/**
 * @brief Baisc linux window implementation that uses X11.
 */
class BEAST_API BasicLinuxWindowPolicy : public IWindowPolicy<BasicLinuxWindowPolicy>
{
  public:
    uint32_t CreateImpl( WindowDesc *pWd );

    void ShowImpl( WindowDesc *pWd );

    void HideImpl( WindowDesc *pWd );

    void DestroyImpl( WindowDesc *pWd );

    void UpdateImpl( WindowDesc *pWd );

  public:
    /**
     * @brief Called last on Create. Use it to query additional functionality from X11.
     */
    virtual void OnCreate( WindowDesc *pWd ) {}

    /**
     * @brief Called first on every Update. Can capture the event or pass it to base class implementation.
     * @return If return value isn't zero, then we immediately escape event processing loop.
     * For examlpe when we just proccesed key press and want it to be immediately dispatched.
     */
    virtual uint32_t OnUpdate( WindowDesc *pWd, XEvent &event );

  private:
    void HandleKey( WindowDesc *pWd, XEvent &event, EAbInputEvents ie );

    void HandleMouseButton( WindowDesc *pWd, XEvent &event, EAbInputEvents ie );
};

} // namespace B33::App
#    endif // !B33_WINDOW_POLICY_H
#endif     // __linux__
