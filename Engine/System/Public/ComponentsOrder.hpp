#ifndef AB_COMPONENTS_ORDER_HPP
#define AB_COMPONENTS_ORDER_HPP

#include "B33System.hpp"

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
    BEAST_API static ComponentOrderRegister RegisterOrder( ::std::vector<::std::string_view> order );
};

/**
* @brief Creates a global instance of ComponentOrderRegister.
* Structure registers that order in to every EngineLoop object.
* Asures that every EngineLoop object will have access to listed components after construction.
* Every operation called on EngineLoop (InitializeComponents, UpdateCompononts, etc...) will loop through
* components in sequence provided in this macro.
*/
#define B33_CREATE_COMPONENTS( ... )                                                                                   \
    static ::B33::System::ComponentOrderRegister g_ComponentOrder =                                                    \
        ::B33::System::ComponentOrderRegister::RegisterOrder( { __VA_ARGS__ } );

} // namespace B33::System
#endif // !AB_COMPONENTS_ORDER_HPP
