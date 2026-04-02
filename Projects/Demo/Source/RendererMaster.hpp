#pragma once

#include "B33System.hpp"
#include "Input/ControllerObject.hpp"
#include "Vulkan/Renderer.hpp"

class RendererMaster
{
  public:
    explicit RendererMaster( ::B33::Rendering::Renderer &renderer )
      : m_RendererInstance( renderer )
      , m_bDebugMode( false )
    {
    }

    ~RendererMaster() = default;

  public:
    bool GetDebugMode() const
    {
        return m_bDebugMode;
    }

  public:
    void SwitchDebugMode( float )
    {
        m_bDebugMode = !m_bDebugMode;
    }

  private:
    ::B33::Rendering::Renderer &m_RendererInstance;
    bool                        m_bDebugMode = false;
};

class RendererMasterController : public ::B33::App::ControllerObject
{
  public:
    RendererMasterController() = default;

    ~RendererMasterController() = default;

  public:
    B33_DECL_ACTION( RendererMaster, SwitchDebugMode, SetWindowMode );
};

class RendererMasterPuppet
{
  public:
    explicit RendererMasterPuppet( ::B33::Rendering::Renderer &renderer )
      : m_Master( renderer )
      , m_Controller()
    {
    }

  public:
    void BindToInput( const ::std::shared_ptr<B33::App::UserInput> &pInput );

    const RendererMaster &GetGameMaster()
    {
        return m_Master;
    }

  private:
    RendererMaster           m_Master;
    RendererMasterController m_Controller;
};
