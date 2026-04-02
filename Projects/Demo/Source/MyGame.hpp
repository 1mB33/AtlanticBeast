#pragma once

#include "Game.hpp"
#include "MainCharacter.hpp"
#include "B33System.hpp"

class MyGame : public ::B33::System::IComponent
{
    B33_ASYNC_COMPONENT( MyGame );

  public:
    MyGame()
      : m_Game()
      , m_Paper( m_Game )
    {
    }

  public:
    virtual void Initialize( ::B33::System::ComponentBridge &bridge ) override;
    virtual void Update( ::B33::System::ComponentBridge &bridge, float fDelta ) override;
    virtual void Destroy( ::B33::System::ComponentBridge &bridge ) override;

  public:
    const Game &GetGameInstance() const
    {
        return m_Game;
    }

    const PlayablePaper &GetMainCharacter() const
    {
        return m_Paper;
    }

  private:
    Game          m_Game = {};
    PlayablePaper m_Paper;
};
