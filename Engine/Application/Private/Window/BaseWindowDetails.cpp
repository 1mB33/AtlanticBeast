#include "B33App.h"

#include "Window/IBaseWindow.hpp"

using namespace std;

#ifdef __linux__

// ---------------------------------------------------------------------------------------------------------------------
struct DisplayCount
{
    ::Display* DisplayPtr;
    size_t Count;
};

// Statics // ----------------------------------------------------------------------------------------------------------
static unordered_map<string, DisplayCount> Displays = { };

// ---------------------------------------------------------------------------------------------------------------------
::Display* AbAskForDisplayLinux(const char* pszDisplayName)
{
    if (pszDisplayName == NULL) {
        pszDisplayName = "";
    }

    if (Displays.find(pszDisplayName) == Displays.end()) {
        Displays[pszDisplayName] = DisplayCount { nullptr, 0 };
    }

    auto& mapped = Displays[pszDisplayName];

    if (mapped.Count == 0) {
        mapped.DisplayPtr = XOpenDisplay(pszDisplayName);
    }

    ++mapped.Count;
    return mapped.DisplayPtr;
}

// ---------------------------------------------------------------------------------------------------------------------
void AbAskToCloseDisplayLinux(const char* pszDisplayName)
{
    if (pszDisplayName == NULL) {
        pszDisplayName = "";
    }

    if (Displays.find(pszDisplayName) == Displays.end()) {
        throw AB_EXCEPT("Traying to close a conection with display that doesn't exists!!!");
    }

    auto& mapped = Displays[pszDisplayName];

    --mapped.Count;

    if (mapped.Count == 0) {
        XCloseDisplay(mapped.DisplayPtr);
    }
}

#elif _WIN32
// Statics // ----------------------------------------------------------------------------------------------------------
static unordered_map<wstring, size_t> RegisteredClasses = { };

// ---------------------------------------------------------------------------------------------------------------------
void AbAskToRegisterWindowClass(const wchar_t* pwszClassName, WNDCLASSEX& wcex)
{
    if (pwszClassName == NULL) {
        pwszClassName = L"";
    }

    if (RegisteredClasses.find(pwszClassName) != RegisteredClasses.end()) {
        ++RegisteredClasses[pwszClassName];
    }

    wstring className = wcex.lpszClassName;

    if (RegisteredClasses.find(className) != RegisteredClasses.end() && 
        RegisteredClasses[className] != 0)
    {
        throw AB_EXCEPT("Trying to register a class that already exists!!!");
    }
   
    ++RegisteredClasses[className];

    if (RegisterClassEx(&wcex) == 0) {
        throw AB_EXCEPT("Failed to register window");
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void AbAskToCloseWindowClass(const wchar_t* pwszClassName)
{
    if (RegisteredClasses.find(wstring(pwszClassName)) == RegisteredClasses.end() || RegisteredClasses[pwszClassName] == 0) {
        throw AB_EXCEPT("Traying to unregister a class that doesn't exists!!!");
    }

    --RegisteredClasses[pwszClassName];
    
    if (RegisteredClasses[pwszClassName] != 0) {
        return;
    }

    UnregisterClass(pwszClassName, GetModuleHandle(NULL));
}

#endif // !__linux__
