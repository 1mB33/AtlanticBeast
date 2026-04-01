#pragma once

#include "B33System.hpp"
#include "Raycaster/VoxelPipeline.hpp"
#include "RendererMaster.hpp"
#include "Vulkan/Renderer.hpp"

class Renderer : public ::B33::System::IComponent
{
    B33_COMPONENT( Renderer );

  public:
    Renderer()
      : m_RendererInstance()
      , m_RendererMaster( m_RendererInstance )
    {
    }

  public:
    virtual void Initialize( ::B33::System::ComponentBridge &bridge ) override;
    virtual void Update( ::B33::System::ComponentBridge &bridge, float fDelta ) override;
    virtual void Destroy( ::B33::System::ComponentBridge &bridge ) override;

  private:
    ::B33::Rendering::Renderer m_RendererInstance = {};
    RendererMasterPuppet       m_RendererMaster;
};
