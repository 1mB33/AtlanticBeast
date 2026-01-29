#include "B33Core.h"

#include "Exception.hpp"

namespace B33::Core
{

using namespace ::std;
using namespace ::B33::Core::Debug;

// ---------------------------------------------------------------------------------------------------------------------
Exception::Exception(const char* szMessage,
                     size_t uMesLen,
                     int32_t uLine,
                     const char* szFileName,
                     size_t uFileNameLen) noexcept 
    : m_pszMessage(szMessage) 
    , m_uMesLen(uMesLen)
    , m_Line(uLine)
    , m_pszFileName(szFileName)
    , m_uFileNameLen(uFileNameLen)
{
    // There is no message to copy
    if (!m_pszMessage) {
        return;
    }

    char szLine[AB_SMALL_STRING] = { 0 };
    char szAtLine[] = "\nAt line: ";
    char szInFile[] = "\nIn file: ";
    size_t uFutureLen = m_uMesLen;
    size_t uCurLen;
 
    // If length is bigger then the AB_LONG_STRING, there is nothing to do, return
    if (m_uMesLen >= AB_LONG_STRING) {
        LogAndReturnMessage(m_pszMessage, m_uMesLen);
        return;
    }

    char* szErrorMessage = new char[AB_LONG_STRING];

    strcpy(szErrorMessage, m_pszMessage);
    m_pszMessage = szErrorMessage;
    
    uCurLen = uFutureLen;
    uFutureLen += sprintf(szLine, "\nAt line: %d", static_cast<int>(m_Line));
    if (uFutureLen >= AB_LONG_STRING || m_Line == InvalidLine) {
        LogAndReturnMessage(szErrorMessage, uCurLen);
        m_uMesLen = uCurLen;
        return;
    }
    strcat(szErrorMessage, szLine);

    uCurLen = uFutureLen;
    uFutureLen += sizeof(szInFile) + m_uFileNameLen;
    if (uFutureLen >= AB_LONG_STRING || !m_pszFileName) {
        LogAndReturnMessage(szErrorMessage, uCurLen);
        m_uMesLen = uCurLen;
        return;
    }
    strcat(szErrorMessage, szInFile);
    strcat(szErrorMessage, m_pszFileName);

    LogAndReturnMessage(szErrorMessage, uFutureLen);
    m_uMesLen = uFutureLen;
}

// ---------------------------------------------------------------------------------------------------------------------
Exception::~Exception() noexcept
{
    if (m_uMesLen < AB_LONG_STRING) {
        delete[] m_pszMessage;
    }
}

// --------------------------------------------------------------------------------------------------------------------
Exception::Exception(const Exception& other) noexcept
    : m_pszMessage(nullptr) 
    , m_uMesLen(other.m_uMesLen)
    , m_Line(other.m_Line)
    , m_pszFileName(other.m_pszFileName)
    , m_uFileNameLen(other.m_uFileNameLen)
{
    if (other.m_uMesLen != InvalidLine && other.m_uMesLen >= AB_LONG_STRING) {
        this->m_pszMessage = other.m_pszMessage;
        this->m_uMesLen = other.m_uMesLen;
        return;
    }
    
    char* pszMessage = new char[AB_LONG_STRING];
    strcpy(pszMessage, other.m_pszMessage);

    this->m_pszMessage = pszMessage;
}

// ---------------------------------------------------------------------------------------------------------------------
const char* Exception::what() const noexcept 
{
    if (m_pszMessage) {
        return m_pszMessage;
    } else {
        return ::std::exception::what();
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void Exception::LogAndReturnMessage(const char* pszMessage, size_t uMesLen) const noexcept
{
    wchar_t* wc = new wchar_t[uMesLen + 1];
    mbstowcs (wc, pszMessage, uMesLen);
    
    try {
        Logger::Get().Log(Error, wc);
        Logger::Get().Flush();
    }
    catch (...) 
    { }
}

} // !B33::Core
