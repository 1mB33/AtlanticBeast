#ifndef AB_EXCEPT_H
#define AB_EXCEPT_H

#include "B33CoreMinimal.h"

namespace B33::Core
{

class BEAST_API Exception : public ::std::exception
{
    
    static constexpr int32_t InvalidLine = -1;

public:

    explicit Exception(const char*  szMessage       = nullptr,
                       size_t       uMesLen         = 0,
                       int32_t      uLine           = InvalidLine,
                       const char*  szFileName      = nullptr,
                       size_t       uFileNameLen    = 0) noexcept;

    template<size_t uMesLen, size_t uFileNameLen>
    constexpr Exception(const char (&pszMessage)[uMesLen], 
                        int32_t    uLine                        = 0,
                        const char (&pszFileName)[uFileNameLen] = nullptr) noexcept
        : Exception(pszMessage, uMesLen, uLine, pszFileName, uFileNameLen)
    { }

    ~Exception() noexcept;

public:

    Exception(const Exception&) noexcept;
    Exception& operator=(const Exception&) noexcept = delete;

    Exception(Exception&&) noexcept = default;
    Exception& operator=(Exception&&) noexcept = delete;

public: 

    virtual const char* what() const noexcept final override;

private:

   void LogAndReturnMessage(const char* pszMessage, size_t uMesLen) const noexcept;

private:

   const char* m_pszMessage;
   size_t m_uMesLen;
   const int32_t m_Line;
   const char* m_pszFileName;
   const size_t m_uFileNameLen;

};

} // !B33::Core
#endif // !AB_EXCEPT_H
