#ifndef AB_ICOMPONENT_H
#define AB_ICOMPONENT_H

namespace B33::System
{

class IComponent
{
  public:
    virtual void Initialize( class ComponentBridge &bridge )           = 0;
    virtual void Update( class ComponentBridge &bridge, float fDelta ) = 0;
    virtual void Destroy( class ComponentBridge &bridge )              = 0;
};

using ComponentInstance = ::std::unique_ptr<IComponent>;
using ComponentFactory  = ComponentInstance ( * )();

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
    static void RegisterInternal( const ::std::string_view &className, ComponentFactory factory );
};

#define B33_COMPONENT( CLASS_NAME )                                                                                    \
  public:                                                                                                              \
    static ::B33::System::ComponentInstance GetComponentFactory()                                                      \
    {                                                                                                                  \
        return ::std::make_unique<CLASS_NAME>();                                                                       \
    }                                                                                                                  \
    static constexpr ::std::string_view GetComponentName()                                                             \
    {                                                                                                                  \
        return #CLASS_NAME;                                                                                            \
    }                                                                                                                  \
                                                                                                                       \
  private:                                                                                                             \
    static inline const ::B33::System::ComponentInstanceRegister RegisteredComponent =                                 \
        ::B33::System::ComponentInstanceRegister::Register( CLASS_NAME::GetComponentName(), &GetComponentFactory );

} // namespace B33::System
#endif // !AB_ICOMPONENT_H
