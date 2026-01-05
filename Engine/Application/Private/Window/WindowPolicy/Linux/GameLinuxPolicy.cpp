#ifdef __linux__

#include "Window/WindowPolicy/Linux/GameLinuxPolicy.hpp"

namespace App
{

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
void GameLinuxWindowPolicy::OnCreate(WindowDesc* pWd)
{
    Display*        pDisplay   = pWd->pDisplayHandle;
    Window          window     = RootWindow(pDisplay, pWd->Screen);
    XIEventMask     evmask;
    unsigned char   pMask[(XI_LASTEVENT + 7) / 8] = { 0 };

    int event, error;
    int major = 2, minor = 0;

    XSync(pDisplay, False);

    if (!XQueryExtension(pDisplay, "XInputExtension", &m_OpCode, &event, &error)) {
        AB_LOG(Core::Debug::Error, L"XInput2 not available.");
        return;
    }

    if (XIQueryVersion(pDisplay, &major, &minor) == BadRequest) {
        AB_LOG(Core::Debug::Error, L"XInput2 isn't available. Need at least 2.0.");
        return;
    }
    AB_LOG(Debug::Info, L"XInput2 version: %d.%d", major, minor);

    XISetMask(pMask, XI_RawMotion);

    evmask.deviceid = XIAllMasterDevices;
    evmask.mask_len = sizeof(pMask);
    evmask.mask     = pMask;

    int status = XISelectEvents(pDisplay, window, &evmask, 1);
    if (status != Success) {
        AB_LOG(Core::Debug::Error, L"XISelectEvents failed.");
    }

    XFlush(pDisplay);
}

// ---------------------------------------------------------------------------------------------------------------------
uint32_t GameLinuxWindowPolicy::OnUpdate(WindowDesc* pWd, XEvent& event)
{
    Display* pDisplay = pWd->pDisplayHandle;

    if (event.xcookie.type      == GenericEvent &&
        event.xcookie.extension == m_OpCode     &&
        XGetEventData(pDisplay, &event.xcookie)) 
    {
        if (event.xcookie.evtype == XI_RawMotion) 
            HandleRawInput(pWd, event);

        XFreeEventData(pDisplay, &event.xcookie);
        return 0;
    }
  
    switch (event.type) {
        case FocusIn:
            HandleFocusIn(pWd);
            return 1;

        case MotionNotify:
            return 0;

    }

    return BasicLinuxWindowPolicy::OnUpdate(pWd, event);
}

// ---------------------------------------------------------------------------------------------------------------------
void GameLinuxWindowPolicy::HandleRawInput(WindowDesc* pWd, XEvent& event)
{
    XIRawEvent* rawev = reinterpret_cast<XIRawEvent*>(event.xcookie.data);
    double dx = 0.0, dy = 0.0;

    for (size_t i = 0; i < rawev->valuators.mask_len * 8; ++i) 
    {
        if (XIMaskIsSet(rawev->valuators.mask, i)) 
        {
            double val = rawev->raw_values[i];

            if (i == 0) 
                dx += val;
            else
                dy += val; 
        }
    }

    pWd->LastEvent |= Input;

    AbInputStruct is;
    is.Event  = AbMotion;
    is.Mouse.MouseX = dx;
    is.Mouse.MouseY = dy;

    pWd->InputStruct.push(is);

    XWarpPointer(pWd->pDisplayHandle, 
                 None, 
                 pWd->WindowHandle,
                 0, 0, 
                 0, 0, 
                 pWd->Width * 0.5f, pWd->Height * 0.5f);
}

// ---------------------------------------------------------------------------------------------------------------------
void GameLinuxWindowPolicy::HandleFocusIn(WindowDesc* pWd)
{ 
    static char pEmptyData[8] = { 0 };
    Display* pDisplay   = pWd->pDisplayHandle;
    Window   window     = pWd->WindowHandle;

    XGrabPointer(pDisplay, 
                 window,
                 True, 
                 PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
                 GrabModeAsync,
                 GrabModeAsync,
                 window,
                 None,
                 CurrentTime);

    Pixmap pixMap;
    Colormap colormap;
    XColor dummyColor;
    Cursor invisibleCursor;

    pixMap = XCreateBitmapFromData(pDisplay, window, pEmptyData, 8, 8);
    invisibleCursor = XCreatePixmapCursor(pDisplay, pixMap, pixMap, &dummyColor, &dummyColor, 0, 0);

    XDefineCursor(pDisplay, window, invisibleCursor);

    XWarpPointer(pDisplay, 
                 None, 
                 window, 
                 0, 0, 
                 0, 0, 
                 pWd->Width * 0.5f, pWd->Height * 0.5f);

    XFlush(pDisplay);
}

} // !App
#endif // !__linux__
