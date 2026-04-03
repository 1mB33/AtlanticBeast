#include "B33Core.h"
#include "B33System.hpp"
#include "ComponentBridge.hpp"
#include "Debug/Assert.hpp"
#include "EngineLoop.hpp"
#include "IComponent.hpp"

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
    for ( IComponent *component : m_AsyncComponents )
        m_JobSystem.PushJob(
            [ = ]()
            {
                component->Update( m_ComponentBridge, fDelta );
            } );

    m_JobSystem.BlockAndWait();

    for ( auto component : m_Components )
        component->Update( m_ComponentBridge, fDelta );
}

void EngineLoop::DestroyComponents()
{
    for ( auto component : m_AsyncComponents )
        component->Destroy( m_ComponentBridge );

    for ( auto component : m_Components )
        component->Destroy( m_ComponentBridge );
}

void EngineLoop::AddComponentInternal( ::std::string_view componentName )
{
    B33_ASSERT_MSG( m_ComponentRegistry.find( componentName ) != m_ComponentRegistry.end(),
                    "That component isn't registered. B33COMPONENT macro might be missing in the class body. " );

    m_ComponentBridge.m_ComponentMap[ componentName ] = m_ComponentRegistry[ componentName ]();

    auto component = ( m_ComponentBridge.m_ComponentMap[ componentName ] ).get();
    switch ( component->GetComponentType() )
    {
        case Abstract:
        {
            throw B33_EXCEPT( "Component type is abstract? Shouldn't be created." );
        }
        case Default:
        {
            if ( m_bInitialized )
                component->Initialize( m_ComponentBridge );

            m_Components.push_back( component );
            break;
        }
        case Async:
        {
            if ( m_bInitialized )
                component->Initialize( m_ComponentBridge );

            m_AsyncComponents.push_back( component );
            break;
        }
        default:
        {
            B33_ASSERT_MSG( false, "Unknown component type" );
        }
    }
}

} // namespace B33::System
