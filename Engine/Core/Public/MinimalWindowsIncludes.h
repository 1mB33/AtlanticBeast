#ifdef _WIN32
#   ifndef AB_MINIMAL_WINDOWS_INLCUDES_H
#   define AB_MINIMAL_WINDOWS_INLCUDES_H
#      define WIN_LEAN_AND_MEAN
#      define NOMINMAX
#      include <Windows.h>
#      include <winDNS.h>
#      include <windowsx.h>   
#   endif // !AB_MINIMAL_WINDOWS_INLCUDES_H
#endif // !_WIN32
