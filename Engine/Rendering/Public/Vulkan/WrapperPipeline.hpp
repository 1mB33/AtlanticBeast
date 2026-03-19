#ifndef AB_WRAPPER_PIPELINE_H
#define AB_WRAPPER_PIPELINE_H

#include "B33Rendering.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class PipelineWrapper
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

  public:
    PipelineWrapper() = delete;

    template <class T>
    PipelineWrapper( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> pAdapter,
                     const ::std::string                                      &strShaderPath,
                     T                                                        *pPipeline )
      : m_pDeviceAdapter( pAdapter )
      , m_DescriptorLayout( pPipeline->CreateDescriptorLayout() )
      , m_DescriptorPool( pPipeline->CreateDescriptorPool() )
      , m_ShaderModule( pPipeline->LoadShader( strShaderPath ) )
      , m_DescriptorSet( pPipeline->CreateDescriptorSet() )
      , m_PipelineLayout( pPipeline->CreatePipelineLayout() )
      , m_Pipeline( pPipeline->CreatePipeline() )
    {
        AB_LOG( Core::Debug::Info, L"Initializing pipeline" );
    }

    ~PipelineWrapper()
    {
        AB_LOG( Core::Debug::Info, L"Destroying pipeline" );
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
    PipelineWrapper( PipelineWrapper && ) noexcept = default;
    PipelineWrapper( const PipelineWrapper & )     = delete;

    PipelineWrapper &operator=( PipelineWrapper && ) noexcept      = default;
    PipelineWrapper &operator=( const PipelineWrapper & ) noexcept = delete;

  public:
    virtual void Update() = 0;

    virtual void RecordCommands( VkCommandBuffer &cmdBuffer ) = 0;

    virtual void Reset() = 0;

    virtual void LoadImage( VkImage image ) = 0;

    virtual void LoadPushConstants( float      fFov,
                                    Vec        cameraPos,
                                    Vec        cameraLookDir,
                                    Vec        cameraRight,
                                    Vec        cameraUp,
                                    ::uint32_t uDebugMode ) = 0;

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

  protected:
    ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> GetAdaterInternal()
    {
        return m_pDeviceAdapter;
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

    ::VkShaderModule        m_ShaderModule     = VK_NULL_HANDLE;
    ::VkDescriptorSetLayout m_DescriptorLayout = VK_NULL_HANDLE;
    ::VkDescriptorPool      m_DescriptorPool   = VK_NULL_HANDLE;
    ::VkDescriptorSet       m_DescriptorSet    = VK_NULL_HANDLE;
    ::VkPipelineLayout      m_PipelineLayout   = VK_NULL_HANDLE;
    ::VkPipeline            m_Pipeline         = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif // !AB_WRAPPER_PIPELINE_H
