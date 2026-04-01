#ifndef AB_ENGINE_LOOP_H
#define AB_ENGINE_LOOP_H

#include "B33System.hpp"
#include "IComponent.hpp"
#include "ComponentsOrder.hpp"
#include "ComponentBridge.hpp"

namespace B33::System
{

class EngineLoop
{
    friend class ComponentInstanceRegister;
    friend class ComponentOrderRegister;

  public:
    EngineLoop()
      : m_Components()
      , m_ComponentBridge()
      , m_bInitialized( false )
    {
    }

    ~EngineLoop() = default;

    EngineLoop( EngineLoop && )      = delete;
    EngineLoop( const EngineLoop & ) = delete;


  public:
    template <class COMPONENT_DERIVED>
    void AddComponent()
    {
        AddComponentInternal( COMPONENT_DERIVED::GetComponentName() );
    }

  public:
    void InitializeComponents();

    void UpdateComponents( float fDelta );

    void DestroyComponents();

  private:
    void AddComponentInternal( ::std::string_view componentName );

  private:
    static inline ::std::unordered_map<::std::string_view, ComponentFactory> m_ComponentRegistry      = {};
    static inline ::std::vector<::std::string_view>                          m_ComponentOrderRegistry = {};

    ::std::vector<IComponent *> m_Components      = {};
    ComponentBridge             m_ComponentBridge = {};

    bool m_bInitialized = false;
};

} // namespace B33::System
#endif // !AB_ENGINE_LOOP_H
