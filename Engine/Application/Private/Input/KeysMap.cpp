#include "B33Core.h"

#include "KeysMap.hpp"

namespace B33::App
{

// --------------------------------------------------------------------------------------------------------------------
KeysMap::KeysMap()
  : m_vKeys( AmountOfBindableKeys )
{
    memset( m_vKeys.data(), 0, sizeof( ActionReplayData ) * m_vKeys.size() );
}

// --------------------------------------------------------------------------------------------------------------------
KeysMap::KeysMap( size_t uAmountOfBindableKeys )
  : m_vKeys( uAmountOfBindableKeys )
{
    memset( m_vKeys.data(), 0, sizeof( ActionReplayData ) * m_vKeys.size() );
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::BindActionImpl( const AbInputBind &ib, void *pThis, AbAction a, AbMouseAction ma )
{
    B33_ASSERT( ib.Type == EAbBindType::Keyboard || ib.Type == EAbBindType::MouseButton );
    B33_ASSERT( ib.Keyboard.KeyCode > B33_INVALID_KEY && ib.Keyboard.KeyCode < B33_KEY_COUNT );
    B33_ASSERT( m_vKeys[ ib.Keyboard.KeyCode ].pThis == nullptr );
    B33_ASSERT( ma == nullptr );
    B33_ASSERT( pThis != nullptr );

    m_vKeys[ ib.Keyboard.KeyCode ] = ActionReplayData { pThis, a };
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::UnbindActionImpl( const AbInputBind &ib, void *pThis )
{
    B33_ASSERT( ib.Type == EAbBindType::Keyboard || ib.Type == EAbBindType::MouseButton );
    B33_ASSERT( ib.Keyboard.KeyCode > B33_INVALID_KEY && ib.Keyboard.KeyCode < B33_KEY_COUNT );
    B33_ASSERT( pThis != nullptr );

    m_vKeys[ ib.Keyboard.KeyCode ].pThis = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::PlayAction( const float fDelta, AbKeyId keyCode )
{
    B33_ASSERT( keyCode > B33_INVALID_KEY && keyCode < B33_KEY_COUNT );

    const auto &playableAction = m_vKeys[ keyCode ];

    if ( playableAction.pThis )
    {
        playableAction.action( fDelta, playableAction.pThis );
    }
}

} // namespace B33::App
