#include "B33Core.h"

#include "Debug/Logger.hpp"

#include <ctime>
#include <iostream>
#include <cstdarg>

namespace B33::Core::Debug
{
 
using namespace ::std;
using namespace ::std::chrono_literals;
using namespace ::B33::Core;

// Logger // -----------------------------------------------------------------------------------------------------------
Logger::Logger()
    : m_InstanceLock()
    , m_MessageQueue()
    , m_strTargetPath(filesystem::current_path().string() + "/Logs/")
    , m_strLogName(CreateDatePreFix() + szLogPostfix)
    , m_aIsWriteThreadWorking(true)
    , m_FlushCondition()
{
    setlocale(LC_ALL, "");
    m_tWriteThreadHandle = thread(&Logger::WriteLoop, this);
}

// ---------------------------------------------------------------------------------------------------------------------
Logger::~Logger()
{
    m_aIsWriteThreadWorking.store(false);
    if (m_tWriteThreadHandle.joinable())
        m_tWriteThreadHandle.join();
}

// ---------------------------------------------------------------------------------------------------------------------
Logger& Logger::Get()
{
    static Logger instance;
    return instance;
}

// ---------------------------------------------------------------------------------------------------------------------
void Logger::Log(const ESeverity sev, const wchar_t wszFmt[], ...)
{
    const auto timeStamp = Clock::now();
    wchar_t* pwszMessage = new wchar_t[AB_LONG_STRING];

    va_list args;
    va_start(args, wszFmt);
    vswprintf(pwszMessage, AB_LONG_STRING, wszFmt, args);
    va_end(args);

    lock_guard<mutex> lock(m_InstanceLock);
    m_MessageQueue.push({ timeStamp, sev, pwszMessage });
}

// -----------------------------------------------------------------------------------------------------------------
void Logger::Flush()
{
    unique_lock<mutex> ul(m_InstanceLock);
    m_FlushCondition.wait(ul, [this]() { return m_MessageQueue.empty(); } );
}

// ---------------------------------------------------------------------------------------------------------------------
string Logger::CreateDatePreFix() const
{
    stringstream    prefix      = { };
    time_t          timeStamp   = time(NULL);
    tm              lTime       = *localtime(&timeStamp);

    prefix << put_time(&lTime, "%Y-%m-%d_%H-%M-%S");

    return prefix.str();
}

// ---------------------------------------------------------------------------------------------------------------------
const wchar_t* Logger::GetTag(const ESeverity sev) const
{
    switch (sev)
    {
        case ESeverity::Info:
            return L"Info";
        case ESeverity::Warning:
            return L"Warning";
        case ESeverity::Error:
            return L"Error";
        default:
            return L"Unknown";
    }
}

// ---------------------------------------------------------------------------------------------------------------------
const wstring Logger::Stringify(const LogStruct& ls) const
{
    time_t  timeStamp   = Clock::to_time_t(ls.TimeStamp);
    tm      lTime       = { 0 };
    tm*     lTimeResult = 
    #ifdef _WIN32
        localtime_s(&timeStamp, &lTime);
    #else
        localtime_r(&timeStamp, &lTime);
    #endif // !_WIN32

    if (lTimeResult == nullptr) 
    {
        lTime.tm_hour   = -1;
        lTime.tm_min    = -1;
        lTime.tm_sec    = -1;
        lTimeResult = &lTime;
    }

    return (wstringstream() << L'[' << put_time(lTimeResult, L"%H:%M:%S") << L"][" 
            << GetTag(ls.Sev) << "]: " << ls.pwszMessage).str();
}

// ---------------------------------------------------------------------------------------------------------------------
void Logger::WriteLoop()
{   
    string      outputPath;
    wofstream   woFile;

    m_InstanceLock.lock();
    
    filesystem::create_directory(m_strTargetPath);
    outputPath = m_strTargetPath + m_strLogName;
    woFile.open(outputPath);
    woFile.close();

    m_InstanceLock.unlock();

    while (true)
    {
        m_InstanceLock.lock();

        if (!m_aIsWriteThreadWorking.load() && m_MessageQueue.empty()) {
            m_InstanceLock.unlock();
            return;
        }
        
        if (m_MessageQueue.empty()) {
            m_FlushCondition.notify_all();
            m_InstanceLock.unlock();
            this_thread::sleep_for(10ms);
            continue;
        }

        LogStruct stamp = m_MessageQueue.front();
        m_MessageQueue.pop();

        m_InstanceLock.unlock();

        wstring wstrStringified = Stringify(stamp);

        wcout << wstrStringified << endl;

        woFile.open(outputPath, ios::app);
        woFile << wstrStringified << endl;
        woFile.close();

        delete[] stamp.pwszMessage;
    }
}

} // !B33::Core
