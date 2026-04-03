#ifndef B33_APP_RESOURCES_H
#define B33_APP_RESOURCES_H

#include "B33Core.h"

namespace B33::App
{

class AppResources
{
    BEAST_API AppResources();

    AppResources( AppResources && ) = delete;

    AppResources( const AppResources & ) = delete;

  public:
    BEAST_API static AppResources &Get();

    ~AppResources() = default;

  public:
    const ::std::wstring &GetExecutablePathW() const
    {
        return m_wstrExePathW;
    }

    BEAST_API ::std::string GetExecutablePathA() const
    {
        return m_strExePathA;
    }

  private:
    ::std::wstring InternalGetExecutablePathW() const;

    ::std::string InternalGetExecutablePathA() const;

  private:
    ::std::wstring m_wstrExePathW;
    ::std::string  m_strExePathA;
};

} // namespace B33::App
#endif // !B33_APP_RESOURCES_H
