#pragma once

#include "Component.hpp"
#include "LinuxEntry.hpp"

B33_ORDER_COMPONENTS( { "Application", "Gamemode", "Renderer" } )

class Application : public IComponent
{
    B33_COMPONENT( Application );

  public:
    virtual void Initialize() override;

    virtual void Update( ComponentBridge &bridge, float fDelta ) override;

    virtual void Destroy() override;
};

class Renderer : public IComponent
{
    B33_COMPONENT( Renderer );

  public:
    virtual void Initialize() override;

    virtual void Update( ComponentBridge &bridge, float fDelta ) override;

    virtual void Destroy() override;
};

class Gamemode : public IComponent
{
    B33_COMPONENT( Gamemode );

  public:
    virtual void Initialize() override;

    virtual void Update( ComponentBridge &bridge, float fDelta ) override;

    virtual void Destroy() override;

    void DoGamemodeThing();

  private:
    int i = 0;
};
