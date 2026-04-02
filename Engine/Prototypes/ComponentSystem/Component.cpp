#include "Component.hpp"

#include <iostream>

using namespace std;

static unordered_map<string_view, ComponentFactory> g_ComponentRegistry      = {};
static vector<string_view>                          g_ComponentOrderRegistry = {};

void ComponentInstanceRegister::RegisterInternal( const ::std::string_view &className, ComponentFactory factory )
{
    g_ComponentRegistry[ className ] = factory;
}

ComponentOrderRegister ComponentOrderRegister::RegisterOrder( ::std::vector<::std::string_view> order )
{
    g_ComponentOrderRegistry = order;
    return ComponentOrderRegister();
}

void EngineLoop()
{
    ComponentBridge      bridge     = ComponentBridge();
    vector<IComponent *> components = {};

    cout << "// ------------------------------------------------" << endl;
    cout << "Registered classes: " << endl;
    for ( auto v : g_ComponentRegistry )
    {
        cout << v.first << endl;
        bridge.GetComponentMap()[ v.first ] = g_ComponentRegistry[ v.first ]();
    }
    cout << "// ------------------------------------------------" << endl;

    for ( auto &componentName : g_ComponentOrderRegistry )
    {
        components.push_back(
            ( bridge.GetComponentMap()[ componentName ] = g_ComponentRegistry[ componentName ]() ).get() );
    }
    for ( auto v : g_ComponentRegistry )
    {
        bool ordered = false;
        for ( auto &componentName : g_ComponentOrderRegistry )
        {
            if ( v.first == componentName )
            {
                ordered = true;
                break;
            }
        }
        if ( !ordered )
        {
            components.push_back( ( bridge.GetComponentMap()[ v.first ] = g_ComponentRegistry[ v.first ]() ).get() );
        }
    }

    for ( auto &component : components )
    {
        component->Initialize();
    }

    for ( int i = 0; i < 3; ++i )
    {
        for ( auto &component : components )
        {
            component->Update( bridge, 1.6f );
        }
    }

    for ( auto &component : components )
    {
        component->Destroy();
    }
}
