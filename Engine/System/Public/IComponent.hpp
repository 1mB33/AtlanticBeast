#ifndef B33_ICOMPONENT_H
#define B33_ICOMPONENT_H

#include "B33System.hpp"

namespace B33::System
{

enum EComponentType
{
    Abstract,
    Default,
    Async,
};

class IComponent;
using ComponentInstance = ::std::unique_ptr<IComponent>;
using ComponentFactory  = ComponentInstance ( * )();

class IComponent
{
  public:
    virtual ::B33::System::EComponentType GetComponentType()
    {
        return ::B33::System::EComponentType::Abstract;
    }

  public:
    virtual ~IComponent() = default;

  public:
    virtual void Initialize( class ComponentBridge &bridge )           = 0;
    virtual void Update( class ComponentBridge &bridge, float fDelta ) = 0;
    virtual void Destroy( class ComponentBridge &bridge )              = 0;
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
        return {};
    }

  private:
    BEAST_API static void RegisterInternal( const ::std::string_view &className, ComponentFactory factory );
};

#define B33_COMPONENT_HELPER( CLASS_NAME )                                                                             \
  public:                                                                                                              \
    static ::B33::System::ComponentInstance GetComponentFactory()                                                      \
    {                                                                                                                  \
        return ::std::make_unique<CLASS_NAME>();                                                                       \
    }                                                                                                                  \
    static ::std::string_view GetComponentName()                                                                       \
    {                                                                                                                  \
        return #CLASS_NAME;                                                                                            \
    }                                                                                                                  \
                                                                                                                       \
  private:                                                                                                             \
    static inline const ::B33::System::ComponentInstanceRegister RegisteredComponent =                                 \
        ::B33::System::ComponentInstanceRegister::Register( #CLASS_NAME, &GetComponentFactory );


#define B33_COMPONENT( CLASS_NAME )                                                                                    \
    B33_COMPONENT_HELPER( CLASS_NAME );                                                                                \
                                                                                                                       \
  public:                                                                                                              \
    virtual ::B33::System::EComponentType GetComponentType() override                                                  \
    {                                                                                                                  \
        return ::B33::System::EComponentType::Default;                                                                 \
    }                                                                                                                  \
                                                                                                                       \
  private:

#define B33_ASYNC_UPDATE_COMPONENT( CLASS_NAME )                                                                       \
    B33_COMPONENT_HELPER( CLASS_NAME );                                                                                \
                                                                                                                       \
  public:                                                                                                              \
    virtual ::B33::System::EComponentType GetComponentType() override                                                  \
    {                                                                                                                  \
        return ::B33::System::EComponentType::Async;                                                                   \
    }                                                                                                                  \
                                                                                                                       \
  private:

} // namespace B33::System
#endif // !B33_ICOMPONENT_H
