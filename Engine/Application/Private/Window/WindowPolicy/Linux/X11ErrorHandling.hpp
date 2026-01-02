#ifdef __linux__
#ifndef AB_X11_ERROR_HANDLING_H
#define AB_X11_ERROR_HANDLING_H

#include "Core.h"

namespace Core 
{

inline int X11HandleError(Display *display, XErrorEvent *error) 
{
    ::std::array<char, AB_MEDIUM_STRING> szErrorMessage;

    XGetErrorText(display, error->error_code, &szErrorMessage[0], szErrorMessage.size());
    ::Core::Debug::Logger::Get().Log(Debug::Error, L"X11 Error: %s", szErrorMessage);

    return 0; 
}

} // !Core


#endif // !AB_X11_ERROR_HANDLING_H
#endif // __linux__
