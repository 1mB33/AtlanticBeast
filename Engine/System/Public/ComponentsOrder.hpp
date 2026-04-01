#ifndef AB_COMPONENTS_ORDER_HPP
#define AB_COMPONENTS_ORDER_HPP

namespace B33::System
{

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
    static ::B33::System::ComponentOrderRegister g_ComponentOrder =                                                    \
        ::B33::System::ComponentOrderRegister::RegisterOrder( __VA_ARGS__ );

} // namespace B33::System
#endif // !AB_COMPONENTS_ORDER_HPP
