#include "App.h"

#include "AppStatus.hpp"

namespace App
{

using namespace std;
using namespace B33::Core;

// --------------------------------------------------------------------------------------------------------------------
EAppStatus AppStatus::m_AppCurrentStatus = EAppStatus::Dead;

// --------------------------------------------------------------------------------------------------------------------
AppStatus::AppStatus()
    : m_uNumberOfWindows(0)
    , m_WindowHandles()
{ }

// --------------------------------------------------------------------------------------------------------------------
AppStatus::~AppStatus()
{ }

// --------------------------------------------------------------------------------------------------------------------
AppStatus& AppStatus::Get()
{
	static AppStatus instance;
	return instance;
}

// --------------------------------------------------------------------------------------------------------------------
EAppStatus AppStatus::GetAppCurrentStatus()
{ return m_AppCurrentStatus; }

// --------------------------------------------------------------------------------------------------------------------
uint32_t AppStatus::SendOpenWindowSignal(shared_ptr<WindowDesc> pWd)
{
    AB_LOG(Debug::Info, L"Got new window signal");

    ++m_uNumberOfWindows;

    UpdateStatus();
    m_WindowHandles.push_back(pWd);

    return  m_uNumberOfWindows;
}

// --------------------------------------------------------------------------------------------------------------------
uint32_t AppStatus::SendCloseWindowSignal(shared_ptr<WindowDesc> pWd)
{
    AB_LOG(Debug::Info, L"Got close window signal");

    m_uNumberOfWindows = m_uNumberOfWindows > 0 ? --m_uNumberOfWindows : m_uNumberOfWindows;
    UpdateStatus();
    m_WindowHandles.remove(pWd);
    
    return  m_uNumberOfWindows;
}

// --------------------------------------------------------------------------------------------------------------------
void AppStatus::UpdateStatus()
{
	AB_LOG(Debug::ESeverity::Info, L"AppStatus updated [Number of active windows: %d]", m_uNumberOfWindows);

    if (m_uNumberOfWindows == 0) {
        m_AppCurrentStatus = EAppStatus::Dead;
        return;
    }

    m_AppCurrentStatus = EAppStatus::Running;
}

} // !App
