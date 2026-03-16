#ifndef AB_PIPELINE_H
#define AB_PIPELINE_H

#include "ExportImport.h"
#include "Vulkan/GPUStreamBuffer.hpp"
#include "Vulkan/Memory.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Raycaster/PushConstants.hpp"
#include "Raycaster/VoxelGrid.hpp"

namespace B33::Rendering
{

class VoxelPipeline
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

  public:
    enum EShaderResource
    {
        VoxelGrid       = 1,
        ObjectPositions = VoxelGrid + 1,
        ObjectRotations = ObjectPositions + 1,
        ObjectHalfSizes = ObjectRotations + 1,
    };

  public:
    BEAST_API VoxelPipeline( ::std::shared_ptr<const ::B33::Rendering::HardwareWrapper> hw,
                             ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>  da,
                             ::std::shared_ptr<::B33::Rendering::Memory>                mem,
                             ::std::shared_ptr<const ::WindowDesc>                      win );

    BEAST_API ~VoxelPipeline();

  public:
    ::VkPipelineLayout GetLayoutHandle() const
    {
        return m_PipelineLayout;
    }

    ::VkPipeline GetPipelineHandle() const
    {
        return m_ComputePipeline;
    }

    ::VkDescriptorSet &GetDescriptorSet()
    {
        return m_DescriptorSet;
    }

    const ::B33::Rendering::VoxelPushConstants &GetPushConstants() const
    {
        return m_Vpc;
    }

  public:
    BEAST_API void CreatePipelineResources( ::std::shared_ptr<::B33::Rendering::CubeWorld> pWorld );

    BEAST_API void Update();

    BEAST_API void RecordCommands( VkCommandBuffer &cmdBuffer );

    BEAST_API void Reset();

    BEAST_API UploadDescriptor
    GetUniformUploadDescriptor( const ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> &outBuffer,
                                const EShaderResource                                      &sr );

    BEAST_API void LoadImage( VkImage image );

    void LoadPushConstants( float      fFov,
                            Vec        cameraPos,
                            Vec        cameraLookDir,
                            Vec        cameraRight,
                            Vec        cameraUp,
                            ::uint32_t uDebugMode )
    {
        ::uint32_t uGridWidth = m_pVoxelGrid->GetGridWidth();

        m_Vpc.CameraPos     = cameraPos;
        m_Vpc.CameraLookDir = cameraLookDir;
        m_Vpc.CameraRight   = cameraRight;
        m_Vpc.CameraUp      = cameraUp;
        m_Vpc.fFov          = fFov;
        m_Vpc.GridSize      = iVec( uGridWidth, uGridWidth, uGridWidth );
        m_Vpc.uMode         = uDebugMode;
    }

  private:
    ::VkDescriptorSetLayout CreateDescriptorLayout( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> &da );

    ::VkDescriptorPool CreateDescriptorPool( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> &da );

    ::VkDescriptorSet CreateDescriptorSet( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> &da,
                                           ::VkDescriptorPool                                         dp,
                                           ::VkDescriptorSetLayout                                    dLayout );

    ::VkPipelineLayout CreatePipelineLayout( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> &da,
                                             ::VkDescriptorSetLayout descriptorSetLayout );

    ::VkShaderModule LoadShader( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> &da,
                                 const ::std::string                                       &strPath );

    ::VkPipeline CreateComputePipeline( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> &da,
                                        ::VkPipelineLayout                                         pipelineLayout,
                                        ::VkShaderModule                                           shaderModule );

  private:
    ::std::shared_ptr<const ::WindowDesc>                      m_pWindowDesc    = nullptr;
    ::std::shared_ptr<const ::B33::Rendering::HardwareWrapper> m_pHardware      = nullptr;
    ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>  m_pDeviceAdapter = nullptr;
    ::std::shared_ptr<::B33::Rendering::Memory>                m_pMemory        = nullptr;
    ::std::shared_ptr<const ::B33::Rendering::Swapchain>       m_pSwapChain     = nullptr;

    ::B33::Rendering::VoxelPushConstants m_Vpc;

    ::VkDescriptorSetLayout m_DescriptorLayout = VK_NULL_HANDLE;
    ::VkDescriptorPool      m_DescriptorPool   = VK_NULL_HANDLE;
    ::VkDescriptorSet       m_DescriptorSet    = VK_NULL_HANDLE;
    ::VkPipelineLayout      m_PipelineLayout   = VK_NULL_HANDLE;
    ::VkShaderModule        m_ShaderModule     = VK_NULL_HANDLE;
    ::VkPipeline            m_ComputePipeline  = VK_NULL_HANDLE;


    // Assets memory
    ::std::shared_ptr<::B33::Rendering::IWorldGrid>      m_pVoxelGrid = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_VoxelBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_PositionsBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_RotationsBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_HalfSizesBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StageVoxelBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StagePositonsBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StageRotationsBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StageHalfSizesBuffer;

    uint32_t m_uStorageBuffersFlags;


    ::VkImageView m_ImageView = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif // !AB_PIPELINE_H
