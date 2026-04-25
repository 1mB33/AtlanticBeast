#ifndef B33_WINDOW_DESC_H
#define B33_WINDOW_DESC_H

#include "B33App.h"
#include "Input/InputEvents.h"
#include "WindowEvents.h"

/**
 * Struct that contains all the handles and information about the window.
 * Can be used to connect with different instances
 * of classes derived from WindowListener (like UserInput) [Engine/Application/Public/Window/WindowListener.hpp]
 * that require window handle.
 */
struct WindowDesc
{
    ::std::wstring              Name;
    const wchar_t              *pwszClassName;
    int32_t                     Width;
    int32_t                     Height;
    bool                        bIsAlive;
    EAbWindowEventsFlags        LastEvent;
    ::std::queue<AbInputStruct> InputStruct;

#if defined( _WIN32 )
    HWND       hWnd;
    WNDCLASSEX Wcex;
#elif defined( _X11 )
    Display *pDisplayHandle;
    Window   WindowHandle;
    int32_t  Screen;
#endif // !_WIN32
};

template <class U>
WindowDesc CreateWindowDesc( U &&wstrName, int32_t width = 1200, int32_t height = 700 )
{
    WindowDesc wd = {};

    wd.Name          = ::std::forward<U>( wstrName );
    wd.pwszClassName = NULL;
    wd.Width         = width;
    wd.Height        = height;
    wd.bIsAlive      = false;
    wd.LastEvent &= 0;

#if defined( _WIN32 )
    wd.hWnd = NULL;
    wd.Wcex = {};
#elif defined( _X11 )
    wd.pDisplayHandle = NULL;
    wd.WindowHandle   = 0;
    wd.Screen         = 0;
#endif // !_WIN32
    return wd;
}

#endif // !B33_WINDOW_DESC_H
