#ifdef _WIN32

#include "Window/WindowPolicy/Win32/WindowModeGameWin32Policy.hpp"

namespace B33::App
{

using namespace std;
using namespace B33::Core;

// ---------------------------------------------------------------------------------------------------------------------
void WindowModeGameWin32WindowPolicy::OnPreWcex()
{
    WindowDesc* pWd = this->GetWindowDesc();

    pWd->pwszClassName = L"GameAtlanticClass";

    memset(&pWd->Wcex, 0, sizeof(WNDCLASSEX));

    pWd->Wcex.cbSize        = sizeof(WNDCLASSEX);
    pWd->Wcex.style         = CS_HREDRAW | CS_VREDRAW;
    pWd->Wcex.hInstance     = GetModuleHandle(NULL);
    pWd->Wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    pWd->Wcex.lpszClassName = pWd->pwszClassName;
    pWd->Wcex.lpfnWndProc   = WindowProc<WindowModeGameWin32WindowPolicy>;
}

// ---------------------------------------------------------------------------------------------------------------------
void WindowModeGameWin32WindowPolicy::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WindowDesc* pWd = this->GetWindowDesc();

    switch (uMsg) {
        case WM_SETFOCUS:
        {
            RAWINPUTDEVICE rid;
            RECT rect;

            rid.usUsagePage = 0x01;
            rid.usUsage = 0x02;
            rid.dwFlags = 0;
            rid.hwndTarget = pWd->hWnd;

            if (!RegisterRawInputDevices(&rid, 1, sizeof(rid))) {
                AB_LOG(B33::Core::Debug::Error, L"Couldn't register raw input");
            }

            ShowCursor(FALSE);

            GetWindowRect(pWd->hWnd, &rect);
            ClipCursor(&rect);
            SetCursorPos(static_cast<int>(rect.left + 0.5f * pWd->Width),
                         static_cast<int>(rect.top + 0.5f * pWd->Height));

            break;
        }

        case WM_KILLFOCUS:
        case WM_DESTROY:
        {
            RAWINPUTDEVICE rid;

            rid.usUsagePage = 0x01;
            rid.usUsage = 0x02;
            rid.dwFlags = RIDEV_REMOVE;
            rid.hwndTarget = NULL;

            if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
                throw AB_EXCEPT("Couldn't register raw input");

            ShowCursor(TRUE);
            ClipCursor(NULL);
            break;
        }

        case WM_INPUT:
        {
            static ::std::vector<BYTE>  vRi         = { };
            AbInputStruct               is          = { };
            UINT                        cbSize;
            UINT                        cbSize2;
            size_t                      uRiRead;
            RAWINPUT*                   pRi;
            RECT                        clientPos;

            if (GetRawInputBuffer(NULL, &cbSize, sizeof(RAWINPUTHEADER)) != 0) {
                AB_LOG(B33::Core::Debug::Error, L"GetRawInputBuffer error %d", GetLastError());
                break;
            }

            if (cbSize == 0)
                break;

            cbSize2 = cbSize * 16; 
            if (vRi.size() < cbSize2)
                vRi.resize(cbSize2);

            uRiRead = GetRawInputBuffer(reinterpret_cast<PRAWINPUT>(&vRi[0]), &cbSize2, sizeof(RAWINPUTHEADER));
            if (uRiRead == static_cast<UINT>(-1)) {
                AB_LOG(B33::Core::Debug::Error, L"GetRawInputBuffer error %d", GetLastError());
                break;
            }

            pWd->LastEvent |= EAbWindowEvents::Input;

            is.Event = EAbInputEvents::AbMotion;
            pRi = reinterpret_cast<PRAWINPUT>(&vRi[0]);
            for (size_t i = 0; 
                 i < uRiRead; 
                 ++i, pRi = NEXTRAWINPUTBLOCK(pRi))
            {
                auto& mouse = pRi->data.mouse;

                if (pRi->header.dwType != RIM_TYPEMOUSE || mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
                    continue;

                is.Mouse.MouseX += mouse.lLastX;
                is.Mouse.MouseY += mouse.lLastY;
            }

            pWd->InputStruct.push(is);

            GetWindowRect(this->GetWindowDesc()->hWnd, &clientPos);

            SetCursorPos(clientPos.left + ((clientPos.right  - clientPos.left) * 0.5f), 
                         clientPos.top  + ((clientPos.bottom - clientPos.top)  * 0.5f));

            return;
        }

        case WM_MOUSEMOVE:
            return;
    }

    return BasicWin32WindowPolicy::OnUpdate(uMsg, wParam, lParam);
}


} // !B33::App
#endif // !_WIN32
