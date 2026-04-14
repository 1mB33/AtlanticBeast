#if defined( __APPLE__ )
#    if !defined( B33_MINIMAL_APPLE_INLCUDES_H )
#        define B33_MINIMAL_APPLE_INLCUDES_H
#        if defined( X11_FOUND )
#            include <X11/XKBlib.h>
#            include <X11/Xlib.h>
#            include <X11/Xutil.h>
#            include <X11/extensions/XInput2.h>
#        endif
#        include <unistd.h>
#    endif
#endif
