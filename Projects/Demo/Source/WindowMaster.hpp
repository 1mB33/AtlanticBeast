#pragma once

#include "EmptyCanvas.hpp"
#include "Input/ControllerObject.hpp"
#include "Window/WindowPolicy/BorderlessGameSystemPolicy.hpp"
#include "Window/WindowPolicy/GameSystemPolicy.hpp"

class WindowMaster
{
  public:
    explicit WindowMaster( ::B33::App::EmptyCanvas<true, B33::App::DefaultGameSystemWindowPolicy> &window )
      : m_Window( window )
    {
    }

  public:
    void SetWindow( const float )
    {
        ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"Changing window display policy to window" );
        m_Window.ChangePolicy<B33::App::DefaultGameSystemWindowPolicy>();
    }

    void SetBorderless( const float )
    {
        ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info,
                                               L"Changing window display policy to borderless" );
        m_Window.ChangePolicy<B33::App::BorderlessGameSystemPolicy>();
    }

    void ExitGame( const float )
    {
        ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"Closing game" );
        ::B33::App::AppStatus::Get().SendExitSignal();
    }

  private:
    B33::App::EmptyCanvas<true, B33::App::DefaultGameSystemWindowPolicy> &m_Window;
};

class WindowMasterController : public B33::App::ControllerObject
{
  public:
    WindowMasterController() = default;

    ~WindowMasterController() = default;

  public:
    B33_DECL_ACTION( WindowMaster, SetWindow, SetWindowMode );
    B33_DECL_ACTION( WindowMaster, SetBorderless, SetBorderless );
    B33_DECL_ACTION( WindowMaster, ExitGame, ExitGame );
};

class WindowMasterPuppet
{
  public:
    explicit WindowMasterPuppet( ::B33::App::EmptyCanvas<true, B33::App::DefaultGameSystemWindowPolicy> &pWindow )
      : m_pGm( pWindow )
      , m_Controller()
    {
    }

  public:
    void BindToInput( const ::std::shared_ptr<B33::App::UserInput> &pInput );

    const WindowMaster &GetGameMaster()
    {
        return m_pGm;
    }

  private:
    WindowMaster           m_pGm;
    WindowMasterController m_Controller;
};
