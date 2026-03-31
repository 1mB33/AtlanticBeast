#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>


class IComponent;
void EngineLoop();

using ComponentInstance = ::std::unique_ptr<IComponent>;
using ComponentFactory  = ComponentInstance ( * )();

class ComponentBridge
{
    friend void EngineLoop();

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
        assert( m_ComponentMap.find( strComponentName ) != m_ComponentMap.end() );
        return *m_ComponentMap[ strComponentName ].get();
    }

    template <class COMPONENT_DERIVED>
    COMPONENT_DERIVED &QueryComponent()
    {
        assert( m_ComponentMap.find( COMPONENT_DERIVED::GetComponentName() ) != m_ComponentMap.end() );
        return dynamic_cast<COMPONENT_DERIVED &>( *m_ComponentMap[ COMPONENT_DERIVED::GetComponentName() ].get() );
    }

  private:
    ::std::unordered_map<::std::string_view, ::std::unique_ptr<IComponent>> &GetComponentMap()
    {
        return m_ComponentMap;
    }

  private:
    ::std::unordered_map<::std::string_view, ::std::unique_ptr<IComponent>> m_ComponentMap = {};
};

struct ComponentInstanceRegister
{
  private:
    ComponentInstanceRegister() = default;

  public:
    ~ComponentInstanceRegister() = default;

    ComponentInstanceRegister( ComponentInstanceRegister && )      = default;
    ComponentInstanceRegister( const ComponentInstanceRegister & ) = default;

    ComponentInstanceRegister &operator=( ComponentInstanceRegister && )      = default;
    ComponentInstanceRegister &operator=( const ComponentInstanceRegister & ) = default;

  public:
    template <class COMPONENT_DERIVED>
    static constexpr ComponentInstanceRegister Register( const ::std::string_view &className )
    {
        return Register( className, &COMPONENT_DERIVED::GetComponentFactory );
    }

    static ComponentInstanceRegister Register( const ::std::string_view &className, ComponentFactory factory )
    {
        RegisterInternal( className, factory );
        return { className };
    }

  public:
    ::std::string_view RegisteredName;

  private:
    static void RegisterInternal( const ::std::string_view &className, ComponentFactory factory );
};

class IComponent
{
  public:
    virtual void Initialize()                                    = 0;
    virtual void Update( ComponentBridge &bridge, float fDelta ) = 0;
    virtual void Destroy()                                       = 0;
};

#define B33_COMPONENT( CLASS_NAME )                                                                                    \
  public:                                                                                                              \
    static ComponentInstance GetComponentFactory()                                                                     \
    {                                                                                                                  \
        return ::std::make_unique<CLASS_NAME>();                                                                       \
    }                                                                                                                  \
    static constexpr ::std::string_view GetComponentName()                                                             \
    {                                                                                                                  \
        return #CLASS_NAME;                                                                                            \
    }                                                                                                                  \
                                                                                                                       \
  private:                                                                                                             \
    static inline const ComponentInstanceRegister RegisteredComponent =                                                \
        ComponentInstanceRegister::Register( CLASS_NAME::GetComponentName(), &GetComponentFactory );

struct ComponentOrderRegister
{
  private:
    ComponentOrderRegister() = default;

  public:
    ~ComponentOrderRegister() = default;

    ComponentOrderRegister( ComponentOrderRegister && )      = default;
    ComponentOrderRegister( const ComponentOrderRegister & ) = default;

    ComponentOrderRegister &operator=( ComponentOrderRegister && )      = default;
    ComponentOrderRegister &operator=( const ComponentOrderRegister & ) = default;

  public:
    static ComponentOrderRegister RegisterOrder( ::std::vector<::std::string_view> order );
};

#define B33_ORDER_COMPONENTS( ... )                                                                                    \
    static ComponentOrderRegister g_ComponentOrder = ComponentOrderRegister::RegisterOrder( __VA_ARGS__ );
