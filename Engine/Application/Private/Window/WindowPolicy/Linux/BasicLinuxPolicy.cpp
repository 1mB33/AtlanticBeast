#include "Input/InputEvents.h"
#ifdef __linux__

#include "B33Core.h"

#include "X11ErrorHandling.hpp"
#include "Window/BaseWindowDetails.h"
#include "Window/WindowPolicy/Linux/BasicLinuxPolicy.hpp"
#include "AppStatus.hpp"

namespace App
{

using namespace B33::Core;

// ---------------------------------------------------------------------------------------------------------------------
uint32_t BasicLinuxWindowPolicy::CreateImpl(WindowDesc* pWd)
{
    XSetErrorHandler(X11HandleError);
    pWd->pDisplayHandle = AbAskForDisplayLinux(NULL);

    if (pWd->pDisplayHandle == NULL) {
        throw AB_EXCEPT("pDisplayHandle is null!");
    }

    int screen = DefaultScreen(pWd->pDisplayHandle);
    Display* pDisplay = pWd->pDisplayHandle;

    Window window = XCreateSimpleWindow(pWd->pDisplayHandle,
                                        RootWindow(pDisplay, screen),
                                        100, 100,
                                        pWd->Width, pWd->Height,
                                        1,
                                        BlackPixel(pDisplay, screen),
                                        WhitePixel(pDisplay, screen));


    XTextProperty windowName;
    char* szWindowName = (char*)malloc(sizeof(char) * AB_SMALL_STRING);
    size_t uWriten = wcstombs(szWindowName, pWd->Name.c_str(), pWd->Name.length());
    szWindowName[uWriten] = '\0';

    XStringListToTextProperty(&szWindowName, 1, &windowName);
    XSetWMName(pDisplay, window, &windowName);
    free(szWindowName);
    XFree(windowName.value);

    int bSupported;
    XkbSetDetectableAutoRepeat(pDisplay, True, &bSupported);
    if (!bSupported) {
        AB_LOG(Debug::Error, L"Detectable auto repeat ISN`T SUPPORTED!");
    }

    XSelectInput(pDisplay, window, FocusChangeMask |
                                   PointerMotionMask |
                                   PointerMotionHintMask |
                                   ButtonPressMask |
                                   ButtonReleaseMask |
                                   KeyPressMask |
                                   KeyReleaseMask |
                                   ExposureMask | 
                                   StructureNotifyMask | 
                                   SubstructureNotifyMask |
                                   SubstructureRedirectMask);
    XMapWindow(pDisplay, window);

    pWd->Screen = screen;
    pWd->WindowHandle = window;

    Atom wmDeleteMessage = XInternAtom(pDisplay, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(pDisplay, window, &wmDeleteMessage, 1);

    OnCreate(pWd);

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::ShowImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd);
    AB_ASSERT(pWd->bIsAlive);
    AB_ASSERT(pWd->pDisplayHandle);
    AB_ASSERT(pWd->WindowHandle);

    XMapWindow(pWd->pDisplayHandle, pWd->WindowHandle);
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::HideImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd);
    AB_ASSERT(pWd->bIsAlive);
    AB_ASSERT(pWd->pDisplayHandle);
    AB_ASSERT(pWd->WindowHandle);

    XUnmapWindow(pWd->pDisplayHandle, pWd->WindowHandle);
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::DestroyImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd);
    AB_ASSERT(pWd->bIsAlive);
    AB_ASSERT(pWd->pDisplayHandle);
    AB_ASSERT(pWd->WindowHandle);

    XDestroyWindow(pWd->pDisplayHandle, pWd->WindowHandle);
    AbAskToCloseDisplayLinux(NULL);

    pWd->WindowHandle   = 0;
    pWd->pDisplayHandle  = NULL;
    pWd->Screen         = 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::UpdateImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd);
    AB_ASSERT(pWd->bIsAlive);
    AB_ASSERT(pWd->pDisplayHandle);
    AB_ASSERT(pWd->WindowHandle);

    Display*    display = pWd->pDisplayHandle;
    Window      window  = pWd->WindowHandle;
    XEvent      event;

    while(XPending(pWd->pDisplayHandle))
    {
        XPeekEvent(display, &event);

        if (event.xany.window != window && 
            event.type != UnmapNotify &&
            event.type != DestroyNotify &&
            event.type != GenericEvent) 
        {
            for (const auto& handle : App::AppStatus::Get().GetWindowHandles()) {
                if (event.xany.window == handle->WindowHandle) {
                    return;
                }
            }
        }

        XNextEvent(display, &event);

        if (OnUpdate(pWd, event) != 0)
            break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
uint32_t BasicLinuxWindowPolicy::OnUpdate(WindowDesc* pWd, XEvent& event)
{
    Display* display = pWd->pDisplayHandle;
    Window   window  = pWd->WindowHandle;

    switch (event.type) {
        case KeyPress:
            HandleKey(pWd, event, AbKeyPress);
            return 0;

        case KeyRelease: 
            HandleKey(pWd, event, AbKeyRelease);
            return 0;

        case ButtonPress:
            HandleMouseButton(pWd, event, AbButtonPress);
            return 0;

        case ButtonRelease:
            HandleMouseButton(pWd, event, AbButtonRelease);
            return 0;

        case MotionNotify:
            int rootX, rootY, dummy;
            Window dummyWindow;

            XQueryPointer(display, 
                          window,
                          &dummyWindow,
                          &dummyWindow,
                          &rootX,
                          &rootY,
                          &dummy,
                          &dummy,
                          reinterpret_cast<unsigned int*>(&dummy));

            pWd->LastEvent |= Input;
            AbInputStruct is;

            is.Event  = AbMotion;
            is.Mouse.MouseX = static_cast<int32_t>(rootX);
            is.Mouse.MouseY = static_cast<int32_t>(rootY);

            pWd->InputStruct.push(is);
            return 0;


        case Expose:
            pWd->LastEvent  |= Resize;
            pWd->Width      = event.xexpose.width;
            pWd->Height     = event.xexpose.height;
            return 1;

        case ConfigureNotify:
            pWd->LastEvent  |= Resize;
            pWd->Height     = event.xconfigure.height;
            pWd->Width      = event.xconfigure.width;
            return 1;

        case ClientMessage:
            Atom wmDeleteMessage;
    
            if (event.xclient.window != window)
                return 1;

            if ((wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", 1)) == None)
                break;

            if (static_cast<Atom>(event.xclient.data.l[0]) == wmDeleteMessage) {
                pWd->LastEvent |= Destroy;
                return 0;
            }

            return 0;
    }

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::HandleKey(WindowDesc* pWd, XEvent& event, EAbInputEvents ie)
{
    pWd->LastEvent |= Input;

    AbInputStruct is;
    is.Event = ie;
    is.Keyboard.KeyId = event.xkey.keycode - 8;

    pWd->InputStruct.push(is);
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::HandleMouseButton(WindowDesc* pWd, XEvent& event, EAbInputEvents ie)
{
    pWd->LastEvent |= Input;

    AbInputStruct is;
    is.Event = ie;
    is.MouseButton.KeyId = event.xbutton.button;

    pWd->InputStruct.push(is);
}

} // !App
#endif // !__linux__
