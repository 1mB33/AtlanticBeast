#ifndef B33_WINDOW_LISTENER_H
#define B33_WINDOW_LISTENER_H

#include "B33Core.h"

#include "Window/WindowDesc.hpp"

namespace B33::App
{

class WindowListener
{
  public:
    explicit WindowListener( ::std::shared_ptr<WindowDesc> pWd = nullptr )
      : m_pWindowDesc( pWd )
    {
    }

    ~WindowListener() = default;

  public:
    WindowListener( const WindowListener &other ) noexcept
      : m_pWindowDesc( other.m_pWindowDesc )
    {
    }

    WindowListener &operator=( const WindowListener &other ) noexcept
    {
        this->m_pWindowDesc = other.m_pWindowDesc;
        return *this;
    }

    WindowListener( WindowListener &&other ) noexcept
      : m_pWindowDesc( std::move( other.m_pWindowDesc ) )
    {
    }

    WindowListener &operator=( WindowListener &&other ) noexcept
    {
        this->m_pWindowDesc = std::move( other.m_pWindowDesc );
        return *this;
    }

  public:
    void ListenToWindow( ::std::shared_ptr<WindowDesc> pWd )
    {
        B33_ASSERT( pWd != nullptr );
        m_pWindowDesc = pWd;
    }

  protected:
    const ::std::shared_ptr<WindowDesc> &GetWindowDesc() const
    {
        B33_ASSERT( m_pWindowDesc != nullptr );
        return m_pWindowDesc;
    }

  private:
    ::std::shared_ptr<WindowDesc> m_pWindowDesc;
};

} // namespace B33::App
#endif //! B33_WINDOW_LISTENER_H
