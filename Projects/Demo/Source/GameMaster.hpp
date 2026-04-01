#pragma once

#include "Game.hpp"
#include "B33Math.hpp"
#include "B33Rendering.hpp"

#include "EmptyCanvas.hpp"
#include "Input/ControllerObject.hpp"
#include "Input/UserInput.hpp"
#include "Vulkan/Renderer.hpp"
#include "Window/WindowPolicy/BorderlessGameSystemPolicy.hpp"
#include "Window/WindowPolicy/GameSystemPolicy.hpp"

class GameMaster
{
  public:
    explicit GameMaster( ::B33::App::EmptyCanvas<> &pWindow, ::B33::Rendering::Renderer &pRenderer )
      : m_pWindow( pWindow )
      , m_pRenderer( pRenderer )
    {
    }

  public:
    void SwitchDebugMode( const float )
    {
        m_pRenderer.SetDebugMode( !m_pRenderer.GetDebugMode() );
    }

    void SetWindowMode( const float )
    {
        ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"Changing window display policy to window" );
        m_pWindow.ChangePolicy<::B33::App::DefaultGameSystemWindowPolicy>();
    }

    void SetBorderless( const float )
    {
        ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info,
                                               L"Changing window display policy to borderless" );
        m_pWindow.ChangePolicy<::B33::App::BorderlessGameSystemPolicy>();
    }

    void ExitGame( const float )
    {
        ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"Closing game" );

        ::B33::App::AppStatus::Get().SendExitSignal();
    }

  private:
    B33::Rendering::Renderer &m_pRenderer;
    B33::App::EmptyCanvas<>  &m_pWindow;
};

class GameMasterController : public B33::App::ControllerObject
{
  public:
    GameMasterController() = default;

    ~GameMasterController() = default;

  public:
    AB_DECL_ACTION( GameMaster, SwitchDebugMode, SwitchDebugMode );
    AB_DECL_ACTION( GameMaster, SetWindowMode, SetWindowMode );
    AB_DECL_ACTION( GameMaster, SetBorderless, SetBorderless );
    AB_DECL_ACTION( GameMaster, ExitGame, ExitGame );
};

class GameMasterPuppet
{
  public:
    explicit GameMasterPuppet( ::B33::App::EmptyCanvas<> &pWindow, B33::Rendering::Renderer &pRenderer )
      : m_pGm( pWindow, pRenderer )
      , m_Controller()
    {
    }

  public:
    void BindToInput( const ::std::shared_ptr<B33::App::UserInput> &pInput );

    const GameMaster &GetGameMaster()
    {
        return m_pGm;
    }

  private:
    GameMaster           m_pGm;
    GameMasterController m_Controller;
};
