#ifdef __linux__
#    include "Window/WindowPolicy/Linux/BorderlessGameLinuxPolicy.hpp"

#    include <X11/X.h>
#    include <X11/Xlib.h>
#    include <X11/Xatom.h>

namespace B33::App
{

using namespace B33::Core;
using namespace B33::Core::Debug;

// ---------------------------------------------------------------------------------------------------------------------
void BorderlessGameLinuxWindowPolicy::OnCreate( WindowDesc *pWd )
{
    GameLinuxWindowPolicy::OnCreate( pWd );


    m_WMState      = XInternAtom( pWd->pDisplayHandle, "_NET_WM_STATE", true );
    m_WMFullscreen = XInternAtom( pWd->pDisplayHandle, "_NET_WM_STATE_FULLSCREEN", true );

    XChangeProperty( pWd->pDisplayHandle,
                     pWd->WindowHandle,
                     m_WMState,
                     XA_ATOM,
                     32,
                     PropModeReplace,
                     reinterpret_cast<unsigned char *>( &m_WMFullscreen ),
                     1 );

    XFlush( pWd->pDisplayHandle );
    XMapWindow( pWd->pDisplayHandle, pWd->WindowHandle );
    XUnmapWindow( pWd->pDisplayHandle, pWd->WindowHandle );

    Window       dummyWindow;
    int          dummyInt;
    unsigned int dummyUInt;

    XGetGeometry( pWd->pDisplayHandle,
                  pWd->WindowHandle,
                  &dummyWindow,
                  &dummyInt,
                  &dummyInt,
                  reinterpret_cast<unsigned int *>( &( pWd->Width ) ),
                  reinterpret_cast<unsigned int *>( &( pWd->Height ) ),
                  &dummyUInt,
                  &dummyUInt );
}

} // namespace B33::App
#endif // !__linux__
