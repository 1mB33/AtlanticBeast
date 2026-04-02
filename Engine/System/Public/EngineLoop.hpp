#ifndef AB_ENGINE_LOOP_H
#define AB_ENGINE_LOOP_H

#include "B33System.hpp"
#include "IComponent.hpp"
#include "ComponentsOrder.hpp"
#include "ComponentBridge.hpp"
#include "Synchronization/JobSystem.hpp"

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
      , m_JobSystem()
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
    BEAST_API void InitializeComponents();

    BEAST_API void UpdateComponents( float fDelta );

    BEAST_API void DestroyComponents();

  private:
    BEAST_API void AddComponentInternal( ::std::string_view componentName );

  private:
    static inline ::std::unordered_map<::std::string_view, ComponentFactory> m_ComponentRegistry      = {};
    static inline ::std::vector<::std::string_view>                          m_ComponentOrderRegistry = {};

    ::std::vector<IComponent *> m_Components      = {};
    ::std::vector<IComponent *> m_AsyncComponents = {};
    ComponentBridge             m_ComponentBridge = {};

    ::B33::Core::JobSystem m_JobSystem = {};


    bool m_bInitialized = false;
};

} // namespace B33::System
#endif // !AB_ENGINE_LOOP_H
