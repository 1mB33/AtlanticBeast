#ifdef _WIN32
#ifndef AB_BORDERLESS_GAME_WIN32_POLICY_H
#define AB_BORDERLESS_GAME_WIN32_POLICY_H

#include "Window/WindowPolicy/Win32/WindowModeGameWin32Policy.hpp"

namespace App
{

/**
 * @brief Borderless fullscreen implementation of WindowModeGameWin32WindowPolicy.
 * 
 * On creation, it queries current display resolution and creates a window
 * that covers the whole screen without borders.
 */
class BorderlessGameWin32Policy : public WindowModeGameWin32WindowPolicy
{
public:

    void OnPreWcex() override
    {
        WindowDesc* pWd = this->GetWindowDesc();

        pWd->pwszClassName = L"BorderlessGameAtlanticClass";

        memset(&pWd->Wcex, 0, sizeof(WNDCLASSEX));

        pWd->Wcex.cbSize		= sizeof(WNDCLASSEX);
        pWd->Wcex.style			= CS_HREDRAW | CS_VREDRAW;
        pWd->Wcex.hInstance		= GetModuleHandle(NULL);
        pWd->Wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
        pWd->Wcex.lpszClassName = pWd->pwszClassName;
        pWd->Wcex.lpfnWndProc	= WindowProc<WindowModeGameWin32WindowPolicy>;

        DEVMODE dm;

        ::ZeroMemory(&dm, sizeof(dm));
        dm.dmSize = sizeof(dm);

        if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm)) {
            pWd->Width = dm.dmPelsWidth;
            pWd->Height = dm.dmPelsHeight;
        }
    }

    void OnPreRegister() override
    {
        WindowDesc* pWd = this->GetWindowDesc();

        HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW,
                                   pWd->pwszClassName,
                                   pWd->Name.c_str(),
                                   WS_POPUP,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   pWd->Width,
                                   pWd->Height,
                                   NULL,
                                   NULL,
                                   GetModuleHandle(NULL),
                                   this);

        if (hWnd == NULL) {
            AB_LOG(Core::Debug::Error, L"Couldn't CreateWindow(), last error %u", GetLastError());
            return;
        }

        pWd->hWnd = hWnd;
    }
};

} // !App
#endif // !AB_BORDERLESS_GAME_WIN32_POLICY_H
#endif // !_WIN32
