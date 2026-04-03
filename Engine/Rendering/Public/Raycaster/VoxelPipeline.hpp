#ifndef B33_PIPELINE_H
#define B33_PIPELINE_H

#include "Vulkan/GPUStreamBuffer.hpp"
#include "Vulkan/IPipeline.hpp"
#include "Vulkan/Memory.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Raycaster/PushConstants.hpp"
#include "Raycaster/VoxelGrid.hpp"

namespace B33::Rendering
{

class VoxelPipeline : public IPipeline<VoxelPipeline>
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

  private:
    enum EShaderResource
    {
        VoxelGrid       = 1,
        ObjectPositions = VoxelGrid + 1,
        ObjectRotations = ObjectPositions + 1,
        ObjectHalfSizes = ObjectRotations + 1,
    };

  public:
    VoxelPipeline( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da,
                   ::std::shared_ptr<::B33::Rendering::Memory>               mem,
                   ::std::shared_ptr<const ::WindowDesc>                     win )
      : IPipeline( da,
                   ::B33::App::AppResources::Get().GetExecutablePathA() + "/Assets/Shaders/Raycast.spv",
                   VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                   VK_PIPELINE_BIND_POINT_COMPUTE )
      , m_pMemory( mem )
      , m_pWindowDesc( win )
      , m_pVoxelGrid( nullptr )
    {
        B33_LOG( Core::Debug::Info, L"Creating a pipeline!" );
    }

    BEAST_API ~VoxelPipeline();

  public:
    BEAST_API virtual void Update() override final;

    BEAST_API virtual void RecordCommands( VkPipelineStageFlagBits lastStage,
                                           VkCommandBuffer        &cmdBuffer ) override final;

    BEAST_API virtual void Reset() override final;

    BEAST_API virtual void LoadImage( VkImage image ) override final;

    ::size_t GetPushConstantsByteSizeImpl()
    {
        return sizeof( m_Vpc );
    }

    IPushConstants *GetPushConstantsImpl()
    {
        return &m_Vpc;
    }

    BEAST_API void CreatePipelineResourcesImpl( ::std::shared_ptr<::B33::Rendering::CubeWorld> pWorld );

    BEAST_API ::VkDescriptorSet CreateDescriptorSetImpl();

    BEAST_API ::VkPipelineLayout CreatePipelineLayoutImpl();

    BEAST_API ::VkPipeline CreatePipelineImpl();

    BEAST_API ::VkDescriptorSetLayout CreateDescriptorLayoutImpl();

    BEAST_API ::VkDescriptorPool CreateDescriptorPoolImpl();

    BEAST_API ::VkShaderModule LoadShaderImpl( const ::std::string &strPath );

  private:
    UploadDescriptor GetUniformUploadDescriptor( const ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> &outBuffer,
                                                 const EShaderResource                                      &sr );

  private:
    ::std::shared_ptr<::B33::Rendering::Memory>          m_pMemory     = nullptr;
    ::std::shared_ptr<const ::WindowDesc>                m_pWindowDesc = nullptr;
    ::std::shared_ptr<const ::B33::Rendering::Swapchain> m_pSwapChain  = nullptr;

    ::B33::Rendering::VoxelPushConstants m_Vpc = {};

    // Shader uniforms
    ::std::shared_ptr<::B33::Rendering::IWorldGrid>      m_pVoxelGrid           = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_VoxelBuffer          = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_PositionsBuffer      = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_RotationsBuffer      = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_HalfSizesBuffer      = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StageVoxelBuffer     = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StagePositonsBuffer  = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StageRotationsBuffer = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StageHalfSizesBuffer = nullptr;

    ::uint32_t m_uStorageBuffersFlags = 0;

    ::VkImageView m_ImageView = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif // !B33_PIPELINE_H
