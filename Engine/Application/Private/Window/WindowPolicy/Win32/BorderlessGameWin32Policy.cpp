#ifdef _WIN32

#    include "Window/BaseWindowDetails.h"
#    include "Window/WindowPolicy/Win32/BasicWin32Policy.hpp"
#    include <windowsx.h>

namespace B33::App
{

using namespace B33::Core;

    void BorderlessGameWin32Policy::OnPreWcex() 
    {
        WindowDesc *pWd = this->GetWindowDesc();

        pWd->pwszClassName = L"BorderlessGameAtlanticClass";

        memset( &pWd->Wcex, 0, sizeof( WNDCLASSEX ) );

        pWd->Wcex.cbSize        = sizeof( WNDCLASSEX );
        pWd->Wcex.style         = CS_HREDRAW | CS_VREDRAW;
        pWd->Wcex.hInstance     = GetModuleHandle( NULL );
        pWd->Wcex.hCursor       = LoadCursor( NULL, IDC_ARROW );
        pWd->Wcex.lpszClassName = pWd->pwszClassName;
        pWd->Wcex.lpfnWndProc   = WindowProc<WindowModeGameWin32WindowPolicy>;

        HMONITOR    hMonitor = MonitorFromWindow( pWd->hWnd, MONITOR_DEFAULTTONEAREST );
        MONITORINFO mi;

        mi.cbSize = sizeof( mi );

        if ( GetMonitorInfo( hMonitor, &mi ) )
        {
            pWd->Width  = mi.rcMonitor.right - mi.rcMonitor.left;
            pWd->Height = mi.rcMonitor.bottom - mi.rcMonitor.top;
        }
    }

    void BorderlessGameWin32Policy::OnPreRegister() 
    {
        WindowDesc *pWd = this->GetWindowDesc();

        HWND hWnd = CreateWindowEx( WS_EX_APPWINDOW,
                                    pWd->pwszClassName,
                                    pWd->Name.c_str(),
                                    WS_POPUP,
                                    CW_USEDEFAULT,
                                    CW_USEDEFAULT,
                                    pWd->Width,
                                    pWd->Height,
                                    NULL,
                                    NULL,
                                    GetModuleHandle( NULL ),
                                    this );

        if ( hWnd == NULL )
        {
            B33_LOG( Core::Debug::Error, L"Couldn't CreateWindow(), last error %u", GetLastError() );
            return;
        }

        pWd->hWnd = hWnd;
    }
} // namespace B33::App
#endif // !_WIN32

