#ifndef AB_BASE_WINDOW_DETAILS_H
#define AB_BASE_WINDOW_DETAILS_H

#include "B33Core.h"

#ifdef __cplusplus
#ifdef __linux__

/**
 * Wraps display query logic for linux.
 **/
BEAST_API Display* AbAskForDisplayLinux(const char* pszDisplayName);

/**
 * Wraps display query logic for linux.
 * Ensures that after window closes, we don't destroy display that is still in use.
 **/
BEAST_API void AbAskToCloseDisplayLinux(const char* pszDisplayName);

#endif // !__linux__

#ifdef _WIN32

/**
 * Wraps window class query logic for win32.
 **/
BEAST_API void AbAskToRegisterWindowClass(const wchar_t* pwszClassName, WNDCLASSEX& wcex);

/**
 * Wraps window class query logic for win32.
 * Ensures that after window closes, we don't unregister window class that is still in use.
 **/
BEAST_API void AbAskToCloseWindowClass(const wchar_t* pwszClassName);

#endif // !_WIN32
#endif // !__cplusplus
#endif // !AB_BASE_WINDOW_DETAILS_H
