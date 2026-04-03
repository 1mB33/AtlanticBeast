#ifndef B33_APPSTATUS_H
#define B33_APPSTATUS_H

#include "B33App.h"

#include "Window/WindowDesc.hpp"

namespace B33::App
{

enum EAppStatus
{
    Dead,
    Running
};

class BEAST_API AppStatus
{
    template <typename T, typename U>
    friend class IBaseWindow;

    AppStatus();

    AppStatus( const AppStatus & ) noexcept            = delete;
    AppStatus &operator=( const AppStatus & ) noexcept = delete;

    AppStatus( AppStatus && ) noexcept            = delete;
    AppStatus &operator=( AppStatus && ) noexcept = delete;

    static EAppStatus m_AppCurrentStatus;

  public:
    static AppStatus &Get();

    ~AppStatus();

  public:
    static EAppStatus GetAppCurrentStatus();

    const ::std::list<::std::shared_ptr<WindowDesc>> &GetWindowHandles() const
    {
        return m_WindowHandles;
    }

    void SendExitSignal();

  private:
    uint32_t SendOpenWindowSignal( ::std::shared_ptr<WindowDesc> pWd );

    uint32_t SendCloseWindowSignal( ::std::shared_ptr<WindowDesc> pWd );

    void UpdateStatus();

  private:
    uint32_t m_uNumberOfWindows;

    ::std::list<::std::shared_ptr<WindowDesc>> m_WindowHandles;
};

} // namespace B33::App
#endif // !B33_APPSTATUS_H
