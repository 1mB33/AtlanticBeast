#ifdef _WIN32

#include "Window/BaseWindowDetails.h"
#include "Window/WindowPolicy/Win32/BasicWin32Policy.hpp"

namespace App
{

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
uint32_t BasicWin32WindowPolicy::CreateImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd != NULL);
    AB_ASSERT(pWd->Hwnd == NULL);
    AB_ASSERT(!pWd->IsAlive);

    m_pWindowDesc = pWd;

    this->OnPreWcex();

    // Fallback to default class name if none is provided,
    // if pwszClassName is provided, we assume that WCEX is already filled
    if (pWd->pwszClassName == NULL || wcscmp(pWd->pwszClassName, L"") == 0)
    {
        pWd->pwszClassName = L"AtlanticClass";

        memset(&pWd->Wcex, 0, sizeof(WNDCLASSEX));

        pWd->Wcex.cbSize            = sizeof(WNDCLASSEX);
        pWd->Wcex.style             = CS_HREDRAW | CS_VREDRAW;
        pWd->Wcex.hInstance         = GetModuleHandle(NULL);
        pWd->Wcex.hCursor           = LoadCursor(NULL, IDC_ARROW);
        pWd->Wcex.lpszClassName     = pWd->pwszClassName;
        pWd->Wcex.lpfnWndProc       = BasicWin32WindowPolicy::WindowProc;
    }

    AbAskToRegisterWindowClass(pWd->pwszClassName, pWd->Wcex);

    this->OnPreRegister();

    if (m_pWindowDesc->Hwnd == NULL)
    {
        HWND hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                                   pWd->pwszClassName,
                                   pWd->Name.c_str(),
                                   WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   pWd->Width,
                                   pWd->Height,
                                   NULL,
                                   NULL,
                                   GetModuleHandle(NULL),
                                   this);

        if (hwnd == NULL) {
            AB_LOG(Core::Debug::Error, L"Couldn't CreateWindow(), last error %u", GetLastError());
            return -1;
        }

        pWd->Hwnd = hwnd;
    }

    ShowWindow(m_pWindowDesc->Hwnd, SW_SHOW);

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::ShowImpl(WindowDesc* pWd)
{ 
    AB_ASSERT(pWd != NULL);
    AB_ASSERT(pWd->Hwnd != NULL);

    ShowWindow(pWd->Hwnd, SW_SHOW);
} 

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::HideImpl(WindowDesc* pWd)
{ 
    AB_ASSERT(pWd != NULL);
    AB_ASSERT(pWd->Hwnd != NULL);

    ShowWindow(pWd->Hwnd, SW_HIDE);
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::DestroyImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd != NULL);
    AB_ASSERT(pWd->Hwnd != NULL);
    AB_ASSERT(pWd->IsAlive);

    if (DestroyWindow(pWd->Hwnd)) {
        pWd->Hwnd = NULL;
    }

    AbAskToCloseWindowClass(pWd->pwszClassName);
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::UpdateImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd != NULL);
    AB_ASSERT(pWd->IsAlive);

    MSG msg;
    while (PeekMessage(&msg, pWd->Hwnd, 0, 0, PM_REMOVE) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_KEYDOWN: {
            // UserInput class handles continuos press by itself, no need to spam
            WORD wKeyFlags = HIWORD(lParam);
            if (wKeyFlags & KF_REPEAT) {
                return;
            }

            AbInputStruct is = { };
            m_pWindowDesc->LastEvent |= EAbWindowEvents::Input;

            is.Event = EAbInputEvents::AbKeyPress;
            is.Keyboard.KeyId = LOWORD(wKeyFlags);

            m_pWindowDesc->InputStruct.push(is);
            return;
        }

        case WM_KEYUP: {
            AbInputStruct is = { };
            m_pWindowDesc->LastEvent |= EAbWindowEvents::Input;

            is.Event = EAbInputEvents::AbKeyRelease;
            is.Keyboard.KeyId = LOWORD(HIWORD(lParam));

            m_pWindowDesc->InputStruct.push(is);
            return;
        }

        case WM_MOUSEMOVE: {
            AbInputStruct is = { };
            m_pWindowDesc->LastEvent |= EAbWindowEvents::Input;

            is.Event = EAbInputEvents::AbMotion;
            is.Mouse.MouseX = GET_X_LPARAM(lParam);
            is.Mouse.MouseY = GET_Y_LPARAM(lParam);

            m_pWindowDesc->InputStruct.push(is);

            return;
        }

        case WM_SIZE:
            m_pWindowDesc->Width   = LOWORD(lParam);
            m_pWindowDesc->Height  = HIWORD(lParam);
            m_pWindowDesc->LastEvent |= EAbWindowEvents::Resize;
            break;

        case WM_CLOSE:
            m_pWindowDesc->LastEvent |= EAbWindowEvents::Destroy;
            break;

        default:
            break;
    }
}

} // !App
#endif // !_WIN32
