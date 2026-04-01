#pragma once

#include "B33System.hpp"
#include "ComponentsOrder.hpp"
#include "EmptyCanvas.hpp"
#include "Window/WindowPolicy/GameSystemPolicy.hpp"

B33_ORDER_COMPONENTS( { "MainWindow", "MyGame", "Renderer" } )

class MainWindow : public ::B33::System::IComponent
{
    B33_COMPONENT( MainWindow );

  public:
    MainWindow()
      : m_WindowInstance( L"Cool Game", 1200, 700 )
    {
    }

  public:
    virtual void Initialize( ::B33::System::ComponentBridge &bridge ) override;
    virtual void Update( ::B33::System::ComponentBridge &bridge, float fDelta ) override;
    virtual void Destroy( ::B33::System::ComponentBridge &bridge ) override;

  public:
    const ::B33::App::EmptyCanvas<true, ::B33::App::DefaultGameSystemWindowPolicy> &GetWindowInstance()
    {
        return m_WindowInstance;
    }

  private:
    ::B33::App::EmptyCanvas<true, ::B33::App::DefaultGameSystemWindowPolicy> m_WindowInstance;
};
