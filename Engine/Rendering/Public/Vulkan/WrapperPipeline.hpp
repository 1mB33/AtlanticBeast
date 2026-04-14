#ifndef B33_WRAPPER_PIPELINE_H
#define B33_WRAPPER_PIPELINE_H

#include "Vulkan/IPushConstants.hpp"
#include "Vulkan/Memory.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class PipelineWrapper
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

  public:
    PipelineWrapper() = delete;

    PipelineWrapper( const ::std::string &strShaderPath, VkPipelineStageFlagBits stage, VkPipelineBindPoint bindPoint )
      : m_pDeviceAdapter( nullptr )
      , m_pMemory( nullptr )
      , m_pWindowDesc( nullptr )
      , m_pSwapChain()
      , m_strShaderPath( strShaderPath )
      , m_StageBits( stage )
      , m_BindPoint( bindPoint )
    {
    }

    ~PipelineWrapper()
    {
        B33_LOG( Core::Debug::Info, L"Destroying pipeline" );
        if ( m_ShaderModule != VK_NULL_HANDLE )
        {
            vkDestroyShaderModule( GetAdaterInternal()->GetAdapterHandle(), m_ShaderModule, NULL );
            m_ShaderModule = VK_NULL_HANDLE;
        }
        if ( m_Pipeline != VK_NULL_HANDLE )
        {
            vkDestroyPipeline( GetAdaterInternal()->GetAdapterHandle(), m_Pipeline, NULL );
            m_Pipeline = VK_NULL_HANDLE;
        }
        if ( m_PipelineLayout != VK_NULL_HANDLE )
        {
            vkDestroyPipelineLayout( GetAdaterInternal()->GetAdapterHandle(), m_PipelineLayout, NULL );
            m_PipelineLayout = VK_NULL_HANDLE;
        }
        if ( m_DescriptorPool != VK_NULL_HANDLE )
        {
            vkDestroyDescriptorPool( GetAdaterInternal()->GetAdapterHandle(), m_DescriptorPool, NULL );
            m_DescriptorPool = VK_NULL_HANDLE;
        }
        if ( m_DescriptorLayout != VK_NULL_HANDLE )
        {
            vkDestroyDescriptorSetLayout( GetAdaterInternal()->GetAdapterHandle(), m_DescriptorLayout, NULL );
            m_DescriptorLayout = VK_NULL_HANDLE;
        }
    }

  public:
    PipelineWrapper( PipelineWrapper && ) noexcept = delete;
    PipelineWrapper( const PipelineWrapper & )     = delete;

    PipelineWrapper &operator=( PipelineWrapper && ) noexcept      = delete;
    PipelineWrapper &operator=( const PipelineWrapper & ) noexcept = delete;

  public:
    template <class T>
    void Initialize( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> pDeviceAdapter,
                     ::std::shared_ptr<::B33::Rendering::Memory>               pMemory,
                     ::std::shared_ptr<const ::WindowDesc>                     pWindowDesc,
                     ::std::shared_ptr<const ::B33::Rendering::Swapchain>      pSwapChain,
                     T                                                        &pPipeline )
    {
        B33_LOG( Core::Debug::Info, L"Initializing pipeline" );

        m_pDeviceAdapter = pDeviceAdapter;
        m_pMemory        = pMemory;
        m_pWindowDesc    = pWindowDesc;
        m_pSwapChain     = pSwapChain;

        m_uPushConstantsByteSize = pPipeline.GetPushConstantsByteSize();
        m_pPushConstants         = pPipeline.GetPushConstants();
        m_DescriptorLayout       = pPipeline.CreateDescriptorLayout();
        m_DescriptorPool         = pPipeline.CreateDescriptorPool();
        m_ShaderModule           = pPipeline.LoadShader( m_strShaderPath );
        m_DescriptorSet          = pPipeline.CreateDescriptorSet();
        m_PipelineLayout         = pPipeline.CreatePipelineLayout();
        m_Pipeline               = pPipeline.CreatePipeline();
    }

    virtual void Update() = 0;

    virtual void RecordCommands( VkPipelineStageFlagBits lastStage, VkCommandBuffer &cmdBuffer ) = 0;

    virtual void Reset() = 0;

    void LoadPushConstants( const IPushConstants &constants, ::size_t uByteSize )
    {
        B33_ASSERT( uByteSize == m_uPushConstantsByteSize );

        memcpy( m_pPushConstants, &constants, m_uPushConstantsByteSize );
    }

  public:
    ::VkPipelineLayout GetLayoutHandle() const
    {
        return m_PipelineLayout;
    }

    ::VkPipeline GetPipelineHandle() const
    {
        return m_Pipeline;
    }

    ::VkDescriptorSet &GetDescriptorSet()
    {
        return m_DescriptorSet;
    }

    ::VkPipelineStageFlagBits GetPipelineStageFlagBits() const
    {
        return m_StageBits;
    }

    ::VkPipelineBindPoint GetPipelineBindPoint() const
    {
        return m_BindPoint;
    }

  public:
    void SetNewSwapChain( ::std::weak_ptr<const ::B33::Rendering::Swapchain> pSwapChain )
    {
        m_pSwapChain = pSwapChain;
    }

  protected:
    const ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> &GetAdaterInternal()
    {
        return m_pDeviceAdapter;
    }

    const ::std::shared_ptr<::B33::Rendering::Memory> &GetMemoryInternal() const
    {
        return m_pMemory;
    }

    const ::std::shared_ptr<const ::WindowDesc> &GetWindowDescInternal() const
    {
        return m_pWindowDesc;
    }

    const ::std::weak_ptr<const ::B33::Rendering::Swapchain> &GetSwapChainInternal() const
    {
        return m_pSwapChain;
    }

    ::VkShaderModule GetShaderModuleInternal()
    {
        return m_ShaderModule;
    }

    ::VkDescriptorSetLayout GetDescriptorLayoutInternal()
    {
        return m_DescriptorLayout;
    }

    ::VkDescriptorPool GetDescriptorPoolInternal()
    {
        return m_DescriptorPool;
    }

  private:
    ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> m_pDeviceAdapter = nullptr;
    ::std::shared_ptr<::B33::Rendering::Memory>               m_pMemory        = nullptr;
    ::std::shared_ptr<const ::WindowDesc>                     m_pWindowDesc    = nullptr;
    ::std::weak_ptr<const ::B33::Rendering::Swapchain>        m_pSwapChain     = {};

    const ::std::string m_strShaderPath          = {};
    ::size_t            m_uPushConstantsByteSize = 0;
    IPushConstants     *m_pPushConstants         = nullptr;

    ::VkPipelineStageFlagBits m_StageBits = {};
    ::VkPipelineBindPoint     m_BindPoint = {};

    ::VkShaderModule        m_ShaderModule     = VK_NULL_HANDLE;
    ::VkDescriptorSetLayout m_DescriptorLayout = VK_NULL_HANDLE;
    ::VkDescriptorPool      m_DescriptorPool   = VK_NULL_HANDLE;
    ::VkDescriptorSet       m_DescriptorSet    = VK_NULL_HANDLE;
    ::VkPipelineLayout      m_PipelineLayout   = VK_NULL_HANDLE;
    ::VkPipeline            m_Pipeline         = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif // !B33_WRAPPER_PIPELINE_H
