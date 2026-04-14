#if defined( X11_FOUND )
#    ifndef B33_X11_ERROR_HANDLING_H
#        define B33_X11_ERROR_HANDLING_H

#        include "B33Core.h"

namespace B33::Core
{

inline int X11HandleError( Display *display, XErrorEvent *error )
{
    ::std::array<char, B33_MEDIUM_STRING> szErrorMessage;

    XGetErrorText( display, error->error_code, &szErrorMessage[ 0 ], szErrorMessage.size() );
    ::B33::Core::Debug::Logger::Get().Log( Debug::Error, L"X11 Error: %s", szErrorMessage );

    return 0;
}

} // namespace B33::Core
#    endif // !B33_X11_ERROR_HANDLING_H
#endif     // __linux__
