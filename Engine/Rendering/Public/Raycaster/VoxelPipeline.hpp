#ifndef AB_PIPELINE_H
#define AB_PIPELINE_H

#include "Voxels.hpp"

#include "Vulkan/GPUStreamBuffer.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Memory.hpp"
#include "Raycaster/PushConstants.hpp"
#include "Raycaster/VoxelGrid.hpp"

namespace Voxels
{

class VoxelPipeline
{
public:

    enum EShaderResource
    {
        VoxelGrid = 1,
        Cubes = VoxelGrid << 1,
    };

public:

    BEAST_API VoxelPipeline(::std::shared_ptr<const HardwareWrapper> hw,
                            ::std::shared_ptr<const AdapterWrapper> da);

    BEAST_API ~VoxelPipeline();

public:

    BEAST_API UploadDescriptor GetUniformUploadDescriptor(const ::std::shared_ptr<GPUStreamBuffer>& outBuffer, 
                                                          const EShaderResource& sr);

    BEAST_API void LoadImage(VkImage image);

    void LoadPushConstants(float fFov,
                           Vec3 cameraPos, 
                           Vec3 cameraLookDir,
                           Vec3 cameraRight,
                           Vec3 cameraUp,
                           int32_t gridWidth,
                           uint32_t uDebugMode)
    {
        m_Vpc.CameraPos     = cameraPos;
        m_Vpc.CameraLookDir = cameraLookDir;
        m_Vpc.CameraRight   = cameraRight;
        m_Vpc.CameraUp      = cameraUp;
        m_Vpc.fFov          = fFov;
        m_Vpc.GridSize      = iVec3(gridWidth, gridWidth, gridWidth);
        m_Vpc.uMode         = uDebugMode;
    }

public:

    VkPipelineLayout GetLayoutHandle() const
    { return m_PipelineLayout; }

    VkPipeline GetPipelineHandle() const
    { return m_ComputePipeline; }

    VkDescriptorSet& GetDescriptorSet()
    { return m_DescriptorSet; }

    const VoxelPushConstants& GetPushConstants() const
    { return m_Vpc; }

private:

    VkDescriptorSetLayout CreateDescriptorLayout(::std::shared_ptr<const AdapterWrapper>& da);

    VkDescriptorPool CreateDescriptorPool(::std::shared_ptr<const AdapterWrapper>& da);

    VkDescriptorSet CreateDescriptorSet(::std::shared_ptr<const AdapterWrapper>& da,
                                        VkDescriptorPool dp,
                                        VkDescriptorSetLayout dLayout);

    VkPipelineLayout CreatePipelineLayout(::std::shared_ptr<const AdapterWrapper>& da,
                                          VkDescriptorSetLayout descriptorSetLayout);

    VkShaderModule LoadShader(::std::shared_ptr<const AdapterWrapper>& da, 
                              const ::std::string& strPath);

    VkPipeline CreateComputePipeline(::std::shared_ptr<const AdapterWrapper>& da, 
                                     VkPipelineLayout pipelineLayout, 
                                     VkShaderModule shaderModule);

private:
    
    ::std::shared_ptr<const HardwareWrapper>    m_pHardware         = nullptr;
    ::std::shared_ptr<const AdapterWrapper>     m_pDeviceAdapter    = nullptr;
    ::std::shared_ptr<const Swapchain>          m_pSwapChain        = nullptr;

    VoxelPushConstants m_Vpc;

    VkDescriptorSetLayout   m_DescriptorLayout  = VK_NULL_HANDLE;
    VkDescriptorPool        m_DescriptorPool    = VK_NULL_HANDLE;
    VkDescriptorSet         m_DescriptorSet     = VK_NULL_HANDLE;
    VkPipelineLayout        m_PipelineLayout    = VK_NULL_HANDLE;
    VkShaderModule		    m_ShaderModule      = VK_NULL_HANDLE;
    VkPipeline              m_ComputePipeline   = VK_NULL_HANDLE;

    VkImageView m_ImageView = VK_NULL_HANDLE;

};

} // !Voxels

#endif // !AB_PIPELINE_H

