#ifndef AB_PIPELINE_H
#define AB_PIPELINE_H

#include "Vulkan/GPUStreamBuffer.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Memory.hpp"
#include "Raycaster/PushConstants.hpp"

namespace B33::Rendering
{

class VoxelPipeline
{

    using Vec   = ::B33::Math::Vec3;
    using iVec  = ::B33::Math::iVec3;

public:

    enum EShaderResource
    {
        VoxelGrid = 1,
        ObjectPositions = VoxelGrid + 1,
        ObjectRotations = ObjectPositions + 1,
        ObjectHalfSizes = ObjectRotations + 1,
    };

public:

    BEAST_API VoxelPipeline(::std::shared_ptr<const ::B33::Rendering::HardwareWrapper> hw,
                            ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da);

    BEAST_API ~VoxelPipeline();

public:

    BEAST_API UploadDescriptor GetUniformUploadDescriptor(const ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer>& outBuffer, 
                                                          const EShaderResource& sr);

    BEAST_API void LoadImage(VkImage image);

    void LoadPushConstants(float fFov,
                           Vec cameraPos, 
                           Vec cameraLookDir,
                           Vec cameraRight,
                           Vec cameraUp,
                           ::int32_t gridWidth,
                           ::uint32_t uDebugMode)
    {
        m_Vpc.CameraPos     = cameraPos;
        m_Vpc.CameraLookDir = cameraLookDir;
        m_Vpc.CameraRight   = cameraRight;
        m_Vpc.CameraUp      = cameraUp;
        m_Vpc.fFov          = fFov;
        m_Vpc.GridSize      = iVec(gridWidth, gridWidth, gridWidth);
        m_Vpc.uMode         = uDebugMode;
    }

public:

    ::VkPipelineLayout GetLayoutHandle() const
    { return m_PipelineLayout; }

    ::VkPipeline GetPipelineHandle() const
    { return m_ComputePipeline; }

    ::VkDescriptorSet& GetDescriptorSet()
    { return m_DescriptorSet; }

    const ::B33::Rendering::VoxelPushConstants& GetPushConstants() const
    { return m_Vpc; }

private:

    ::VkDescriptorSetLayout CreateDescriptorLayout(::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>& da);

    ::VkDescriptorPool CreateDescriptorPool(::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>& da);

    ::VkDescriptorSet CreateDescriptorSet(::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>& da,
                                          ::VkDescriptorPool dp,
                                          ::VkDescriptorSetLayout dLayout);

    ::VkPipelineLayout CreatePipelineLayout(::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>& da,
                                            ::VkDescriptorSetLayout descriptorSetLayout);

    ::VkShaderModule LoadShader(::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>& da, 
                                const ::std::string& strPath);

    ::VkPipeline CreateComputePipeline(::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>& da, 
                                       ::VkPipelineLayout pipelineLayout, 
                                       ::VkShaderModule shaderModule);

private:
    
    ::std::shared_ptr<const ::B33::Rendering::HardwareWrapper>    m_pHardware         = nullptr;
    ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>     m_pDeviceAdapter    = nullptr;
    ::std::shared_ptr<const ::B33::Rendering::Swapchain>          m_pSwapChain        = nullptr;

    ::B33::Rendering::VoxelPushConstants m_Vpc;

    ::VkDescriptorSetLayout   m_DescriptorLayout  = VK_NULL_HANDLE;
    ::VkDescriptorPool        m_DescriptorPool    = VK_NULL_HANDLE;
    ::VkDescriptorSet         m_DescriptorSet     = VK_NULL_HANDLE;
    ::VkPipelineLayout        m_PipelineLayout    = VK_NULL_HANDLE;
    ::VkShaderModule		  m_ShaderModule      = VK_NULL_HANDLE;
    ::VkPipeline              m_ComputePipeline   = VK_NULL_HANDLE;

    ::VkImageView m_ImageView = VK_NULL_HANDLE;

};

} // !B33::Rendering
#endif // !AB_PIPELINE_H
