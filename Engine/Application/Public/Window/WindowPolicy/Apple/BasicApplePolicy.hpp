#if defined( __APPLE__ )
#    ifndef B33_WINDOW_POLICY_H
#        define B33_WINDOW_POLICY_H

#        include "Window/WindowPolicy/IWindowPolicy.hpp"

namespace B33::App
{

/**
 * @brief Baisc linux window implementation that uses X11.
 */
class BEAST_API BasicAppleWindowPolicy : public IWindowPolicy<BasicAppleWindowPolicy>
{
  public:
    uint32_t CreateImpl( WindowDesc *pWd );

    void ShowImpl( WindowDesc *pWd );

    void HideImpl( WindowDesc *pWd );

    void DestroyImpl( WindowDesc *pWd );

    void UpdateImpl( WindowDesc *pWd );
};

} // namespace B33::App
#    endif // !B33_WINDOW_POLICY_H
#endif     // __APPLE__
