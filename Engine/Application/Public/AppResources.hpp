#ifndef AB_APP_RESOURCES_H
#define AB_APP_RESOURCES_H

#include "Core.h"

namespace App
{

class AppResources
{
    BEAST_API AppResources();

    AppResources(AppResources&&) = delete;
    
    AppResources(const AppResources&) = delete;

public:

    BEAST_API static AppResources& Get();

    ~AppResources() = default;

public:

    const ::std::wstring& GetExecutablePathW() const
    { return m_wstrExePathW; }

    BEAST_API ::std::string GetExecutablePathA() const
    { return m_strExePathA; }

private:

    ::std::wstring InternalGetExecutablePathW() const;

    ::std::string InternalGetExecutablePathA() const;

private:

    ::std::wstring m_wstrExePathW;
    ::std::string m_strExePathA;

};

} // !App

#endif // !AB_APP_RESOURCES_H
