#include "B33Core.h"

#include "MouseMap.hpp"

namespace B33::App
{

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::BindActionImpl( const AbInputBind &ib, void *pThis, AbAction a, AbMouseAction ma )
{
    B33_ASSERT( ib.Type == EAbBindType::Mouse );
    B33_ASSERT( a == nullptr );
    B33_ASSERT( pThis != nullptr );

    m_vMouseBinds.push_back( DataForActionReplay { pThis, ma } );
}

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::UnbindActionImpl( const AbInputBind &ib, void *pThis )
{
    B33_ASSERT( ib.Type == EAbBindType::Mouse );
    B33_ASSERT( pThis != nullptr );

    auto it = m_vMouseBinds.begin();
    for ( ; it != m_vMouseBinds.end(); ++it )
    {
        if ( it->pThis == pThis )
        {
            m_vMouseBinds.erase( it );
            return;
        }
    }

    B33_ASSERT( it != m_vMouseBinds.end() );
}

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::PlayAction( const float fDelta, int32_t fX, int32_t fY )
{
    for ( const auto &bind : m_vMouseBinds )
    {
        B33_ASSERT( bind.pThis );

        bind.Action( fDelta, bind.pThis, fX, fY );
    }
}

} // namespace B33::App
