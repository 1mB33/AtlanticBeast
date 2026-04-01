#include "B33System.hpp"
#include "EngineLoop.hpp"

namespace B33::System
{

using namespace std;
using namespace B33;

void EngineLoop::InitializeComponents()
{
    m_bInitialized = true;
    m_Components.clear();

    for ( auto &requiredComponent : m_ComponentOrderRegistry )
        AddComponentInternal( requiredComponent );
}

void EngineLoop::UpdateComponents( float fDelta )
{
    for ( auto component : m_Components )
        component->Update( m_ComponentBridge, fDelta );
}

void EngineLoop::DestroyComponents()
{
    for ( auto component : m_Components )
        component->Destroy();
}

void EngineLoop::AddComponentInternal( ::std::string_view componentName )
{
    AB_ASSERT_MSG( m_ComponentRegistry.find( componentName ) != m_ComponentRegistry.end(),
                   "That component isn't registered. B33COMPONENT macro might be missing in the class body. " );

    auto component =
        ( m_ComponentBridge.m_ComponentMap[ componentName ] = m_ComponentRegistry[ componentName ]() ).get();

    m_Components.push_back( component );

    if ( m_bInitialized )
    {
        component->Initialize();
    }
}

} // namespace B33::System
