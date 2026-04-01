#ifndef AB_RENDERER_H
#define AB_RENDERER_H

#include "B33Rendering.hpp"

#include "ExportImport.h"
#include "Vulkan/FrameResources.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/WrapperAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Memory.hpp"
#include "Vulkan/WrapperPipeline.hpp"

namespace B33::Rendering
{

class Renderer
{
    using FramesArray = ::std::array<::B33::Rendering::Frame, ::B33::Rendering::Frame::MAX_FRAMES_IN_FLIGHT>;

  public:
    Renderer()
      : m_pInstance( nullptr )
      , m_pHardware( nullptr )
      , m_pDeviceAdapter( nullptr )
      , m_pWindowDesc( nullptr )
      , m_pSwapChain( nullptr )
      , m_pMemory( nullptr )
      , m_vPipeline()
      , m_CommandPool( VK_NULL_HANDLE )
      , m_uCurrentFrame( 0 )
      , m_vFrames()
    {
    }

    ~Renderer()
    {
        Destroy();
    }

  public:
    BEAST_API void Initialize( ::std::shared_ptr<const ::WindowDesc> wd );

    BEAST_API void Update( const float fDelta );

    BEAST_API void Render();

    BEAST_API void Destroy();

    template <class PIPE_LINE = ::B33::Rendering::PipelineWrapper, class... RESOURCES_ARGS>
    void PushPipeline( RESOURCES_ARGS... args )
    {
        auto pipeline = ::std::make_shared<PIPE_LINE>( ::std::static_pointer_cast<AdapterWrapper>( m_pDeviceAdapter ),
                                                       m_pMemory,
                                                       m_pWindowDesc );
        pipeline->Initialize( *pipeline );
        pipeline->CreatePipelineResources( args... );

        m_vPipeline.push_back( ::std::static_pointer_cast<::B33::Rendering::PipelineWrapper>( pipeline ) );
    }

    ::std::shared_ptr<::B33::Rendering::PipelineWrapper> &GetPipeline( const ::size_t uIndex )
    {
        return m_vPipeline[ uIndex ];
    }

  private:
    ::VkCommandPool CreateCommandPool( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da,
                                       ::uint32_t                                                uQueueFamily );

    ::VkCommandBuffer CreateCommandBuffer( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da,
                                           ::VkCommandPool                                           cmdPool );

    FramesArray CreateFrameResources( const ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> &da,
                                      const ::std::shared_ptr<::B33::Rendering::Memory>               &memory,
                                      ::VkCommandPool                                                  cmdPool,
                                      ::size_t                                                         uFrames );

    void RecordCommands( ::VkCommandBuffer &cmdBuff, ::uint32_t uImageIndex );

  private:
    void DestroyFrameResources();

    void RecreateSwapChain();

  private:
    ::std::shared_ptr<const ::WindowDesc>                m_pWindowDesc    = nullptr;
    ::std::shared_ptr<::B33::Rendering::Instance>        m_pInstance      = nullptr;
    ::std::shared_ptr<::B33::Rendering::HardwareWrapper> m_pHardware      = nullptr;
    ::std::shared_ptr<::B33::Rendering::AdapterWrapper>  m_pDeviceAdapter = nullptr;
    ::std::unique_ptr<::B33::Rendering::Swapchain>       m_pSwapChain     = nullptr;
    ::std::shared_ptr<::B33::Rendering::Memory>          m_pMemory        = nullptr;

    ::std::vector<::std::shared_ptr<::B33::Rendering::PipelineWrapper>> m_vPipeline = {};

    ::VkCommandPool m_CommandPool = VK_NULL_HANDLE;

    ::size_t                       m_uCurrentFrame;
    ::std::unique_ptr<FramesArray> m_vFrames = nullptr;
};

} // namespace B33::Rendering
#endif // !AB_RENDERER_H
