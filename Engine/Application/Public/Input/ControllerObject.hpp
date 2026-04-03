#ifndef B33_CONTROLLER_OBJECT_H
#define B33_CONTROLLER_OBJECT_H

#include "B33Core.h"

#include "Bind.h"
#include "Input/UserInput.hpp"

namespace B33::App
{

class ControllerObject
{
    friend UserInput;

  public:
    ControllerObject() = default;

    ~ControllerObject()
    {
        if ( auto pUserInput = m_pUserInput.lock() )
            pUserInput->Unbind( this );
    }

  public:
    ControllerObject( const ControllerObject & ) noexcept            = delete;
    ControllerObject &operator=( const ControllerObject & ) noexcept = delete;

    ControllerObject( ControllerObject &&other ) noexcept
      : m_pUserInput( std::move( other.m_pUserInput ) )
    {
    }

  public:
    void SignObject( ::std::weak_ptr<UserInput> pUserInput )
    {
        // We can be signed by only one UserInput
        if ( !m_pUserInput.expired() )
        {
            B33_ASSERT( m_pUserInput.lock().get() == pUserInput.lock().get() );
            B33_LOG( Core::Debug::Warning, L"ControllerObject can be signed only by one UserInput." );
            return;
        }

        m_pUserInput = pUserInput;
    }

  private:
    ::std::weak_ptr<UserInput> m_pUserInput;
};

} // namespace B33::App

#define B33_DECL_ACTION( baseClass, action, customName, ... )                                                          \
    static ::AbActionType UseAction##customName( const float fDelta, void *pThis )                                     \
    {                                                                                                                  \
        B33_ASSERT( pThis != nullptr );                                                                                \
                                                                                                                       \
        static_cast<baseClass *>( pThis )->action( fDelta B33_VA_ARGS_( __VA_ARGS__ ) );                               \
        return ::AbActionType();                                                                                       \
    }

#define B33_DECL_MOUSE_ACTION( baseClass, action, customName )                                                         \
    static ::AbActionType UseAction##customName( const float fDelta, void *pThis, int32_t fX, int32_t fY )             \
    {                                                                                                                  \
        B33_ASSERT( pThis != nullptr );                                                                                \
                                                                                                                       \
        static_cast<baseClass *>( pThis )->action( fDelta, fX, fY );                                                   \
        return ::AbActionType();                                                                                       \
    }

#endif // !B33_CONTROLLER_OBJECT_H
