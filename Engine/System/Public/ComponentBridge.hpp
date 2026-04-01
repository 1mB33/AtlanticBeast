#ifndef AB_COMPONENT_BRIDGE_H
#define AB_COMPONENT_BRIDGE_H

#include "IComponent.hpp"

namespace B33::System
{

class ComponentBridge
{
    friend class EngineLoop;

  public:
    ComponentBridge()
      : m_ComponentMap()
    {
    }

    ~ComponentBridge() = default;

    ComponentBridge( const ComponentBridge & ) = delete;
    ComponentBridge( ComponentBridge && )      = delete;

    ComponentBridge &operator=( const ComponentBridge & ) = delete;
    ComponentBridge &operator=( ComponentBridge && )      = delete;

  public:
    IComponent &QueryComponent( ::std::string strComponentName )
    {
        AB_ASSERT( m_ComponentMap.find( strComponentName ) != m_ComponentMap.end() );
        return *m_ComponentMap[ strComponentName ].get();
    }

    template <class COMPONENT_DERIVED>
    COMPONENT_DERIVED &QueryComponent()
    {
        AB_ASSERT( m_ComponentMap.find( COMPONENT_DERIVED::GetComponentName() ) != m_ComponentMap.end() );
        return dynamic_cast<COMPONENT_DERIVED &>( *m_ComponentMap[ COMPONENT_DERIVED::GetComponentName() ].get() );
    }

  private:
    ::std::unordered_map<::std::string_view, ::std::unique_ptr<IComponent>> m_ComponentMap = {};
};

} // namespace B33::System
#endif // !AB_COMPONENT_BRIDGE_H
