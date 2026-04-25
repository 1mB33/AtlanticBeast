#if !defined( B33_EDITOR_PIPELINE_HPP )
#    define B33_EDITOR_PIPELINE_HPP

#    include "Vulkan/IPipeline.hpp"
#    include "Vulkan/Memory.hpp"
#    include "Vulkan/SwapChain.hpp"
#    include "Editor/PushConstants.hpp"

namespace B33::Rendering
{

class EditorPipeline : public IPipeline<EditorPipeline>
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

  public:
    EditorPipeline()
      : IPipeline( VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_BIND_POINT_COMPUTE )
      , m_ShaderModule( VK_NULL_HANDLE )
    {
        B33_LOG( Core::Debug::Info, L"Creating a editor pipeline!" );
    }

    BEAST_API ~EditorPipeline();

  public:
    BEAST_API virtual void Update() override final;

    BEAST_API virtual void RecordCommands( VkPipelineStageFlagBits lastStage,
                                           VkCommandBuffer        &cmdBuffer ) override final;

    BEAST_API virtual void Reset() override final;

    ::size_t GetPushConstantsByteSizeImpl()
    {
        return sizeof( m_Vpc );
    }

    IPushConstants *GetPushConstantsImpl()
    {
        return &m_Vpc;
    }

    BEAST_API void CreatePipelineResourcesImpl();

    BEAST_API ::VkDescriptorSet CreateDescriptorSetImpl();

    BEAST_API ::VkPipelineLayout CreatePipelineLayoutImpl();

    BEAST_API ::VkPipeline CreatePipelineImpl();

    BEAST_API ::VkDescriptorSetLayout CreateDescriptorLayoutImpl();

    BEAST_API ::VkDescriptorPool CreateDescriptorPoolImpl();

  private:
    void LoadImage( VkImage image );

    BEAST_API ::VkShaderModule LoadShader( const ::std::string &strPath );

  private:
    ::B33::Rendering::EditorPushConstants m_Vpc = {};

    ::VkShaderModule m_ShaderModule = VK_NULL_HANDLE;

    ::uint32_t m_uStorageBuffersFlags = 0;

    ::VkImageView m_ImageView = VK_NULL_HANDLE;
};

} // namespace B33::Rendering

#endif // !B33_EDITOR_PIPELINE_HPP
