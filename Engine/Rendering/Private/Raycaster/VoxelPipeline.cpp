#include "B33Rendering.hpp"

#include "Raycaster/VoxelPipeline.hpp"

#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/GPUStreamBuffer.hpp"
#include "Vulkan/Memory.hpp"
#include "Vulkan/SwapChain.hpp"

namespace B33::Rendering
{

using namespace ::std;
using namespace ::B33::Math;

// ---------------------------------------------------------------------------------------------------------------------
VoxelPipeline::VoxelPipeline(::std::shared_ptr<const HardwareWrapper> hw, 
                             ::std::shared_ptr<const AdapterWrapper> da)
    : m_pHardware(hw)
    , m_pDeviceAdapter(da)
    , m_DescriptorLayout(CreateDescriptorLayout(m_pDeviceAdapter))
    , m_DescriptorPool(CreateDescriptorPool(m_pDeviceAdapter))
    , m_DescriptorSet(CreateDescriptorSet(m_pDeviceAdapter, m_DescriptorPool, m_DescriptorLayout))
    , m_PipelineLayout(CreatePipelineLayout(m_pDeviceAdapter, m_DescriptorLayout))
    , m_ShaderModule(LoadShader(m_pDeviceAdapter, 
                                App::AppResources::Get().GetExecutablePathA() + "/Assets/Shaders/RaycastAABB.comp.spv"))
    , m_ComputePipeline(CreateComputePipeline(m_pDeviceAdapter, m_PipelineLayout, m_ShaderModule))
{
    AB_LOG(Core::Debug::Info, L"Creating a pipeline!");
}

// ---------------------------------------------------------------------------------------------------------------------
VoxelPipeline::~VoxelPipeline()
{
    AB_LOG(Core::Debug::Info, L"Destroying pipeline");

    if (m_ImageView != VK_NULL_HANDLE) {
        vkDestroyImageView(m_pDeviceAdapter->GetAdapterHandle(), m_ImageView, nullptr);
        m_ImageView = VK_NULL_HANDLE;
    }
    if (m_ComputePipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(m_pDeviceAdapter->GetAdapterHandle(), m_ComputePipeline, NULL);
        m_ComputePipeline = VK_NULL_HANDLE;
    }
    if (m_ShaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(m_pDeviceAdapter->GetAdapterHandle(), m_ShaderModule, NULL);
        m_ShaderModule = VK_NULL_HANDLE;
    }
    if (m_PipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(m_pDeviceAdapter->GetAdapterHandle(), m_PipelineLayout, NULL);
        m_PipelineLayout = VK_NULL_HANDLE;
    }
    if (m_DescriptorPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(m_pDeviceAdapter->GetAdapterHandle(), m_DescriptorPool, NULL);
        m_DescriptorPool = VK_NULL_HANDLE;
    }
    if (m_DescriptorLayout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(m_pDeviceAdapter->GetAdapterHandle(), m_DescriptorLayout, NULL);
        m_DescriptorLayout = VK_NULL_HANDLE;
    }
}

// Public // -----------------------------------------------------------------------------------------------------------
UploadDescriptor VoxelPipeline::GetUniformUploadDescriptor(const shared_ptr<GPUStreamBuffer>& outBuffer, 
                                                           const EShaderResource& sr)
{ 
    VkDescriptorBufferInfo bufferInfo = { };
    bufferInfo.buffer  = outBuffer->GetBufferHandle();
    bufferInfo.offset  = 0;
    bufferInfo.range   = VK_WHOLE_SIZE;

    VkWriteDescriptorSet write = { };
    write.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet           = m_DescriptorSet;
    write.dstBinding       = sr;
    write.descriptorType   = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    write.descriptorCount  = 1;
    write.pBufferInfo      = &bufferInfo;

    return UploadDescriptor(std::move(bufferInfo), 
                            std::move(write),
                            UploadDescriptor::EUploadType::StreamBuffer,
                            outBuffer);
}

// ---------------------------------------------------------------------------------------------------------------------
void VoxelPipeline::LoadImage(VkImage image)
{
    if (m_ImageView != VK_NULL_HANDLE) {
        vkDestroyImageView(m_pDeviceAdapter->GetAdapterHandle(), m_ImageView, nullptr);
        m_ImageView = VK_NULL_HANDLE;
    }

    VkImageViewCreateInfo viewInfo = { };
    viewInfo.sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image      = image;
    viewInfo.viewType   = VK_IMAGE_VIEW_TYPE_2D; 
    viewInfo.format     = Swapchain::TargetedFormat;
    viewInfo.subresourceRange.aspectMask        = VK_IMAGE_ASPECT_COLOR_BIT; 
    viewInfo.subresourceRange.baseMipLevel      = 0;
    viewInfo.subresourceRange.levelCount        = 1;
    viewInfo.subresourceRange.baseArrayLayer    = 0;
    viewInfo.subresourceRange.layerCount        = 1;

    THROW_IF_FAILED(vkCreateImageView(m_pDeviceAdapter->GetAdapterHandle(), 
                                      &viewInfo,
                                      NULL,
                                      &m_ImageView));

    VkDescriptorImageInfo imageInfo = { };
    imageInfo.imageView     = m_ImageView;
    imageInfo.imageLayout   = VK_IMAGE_LAYOUT_GENERAL; 

    VkWriteDescriptorSet imageWrite = { };
    imageWrite.sType    = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    imageWrite.dstSet   = m_DescriptorSet;
    imageWrite.dstBinding       = 0; 
    imageWrite.dstArrayElement  = 0;
    imageWrite.descriptorType   = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    imageWrite.descriptorCount  = 1;
    imageWrite.pImageInfo       = &imageInfo;
    
    vkUpdateDescriptorSets(m_pDeviceAdapter->GetAdapterHandle(), 
                           1,
                           &imageWrite,
                           0,
                           NULL);
}


// Private // ----------------------------------------------------------------------------------------------------------
VkDescriptorSetLayout VoxelPipeline::CreateDescriptorLayout(shared_ptr<const AdapterWrapper>& da)
{
    array<VkDescriptorSetLayoutBinding, 5>  bindings    = { };
    VkDescriptorSetLayout                   descriptorSetLayout;

    bindings[0].binding             = 0;
    bindings[0].descriptorType      = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    bindings[0].descriptorCount     = 1;
    bindings[0].stageFlags          = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[1].binding             = VoxelPipeline::EShaderResource::VoxelGrid;
    bindings[1].descriptorType      = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[1].descriptorCount     = 1;
    bindings[1].stageFlags          = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[2].binding             = VoxelPipeline::EShaderResource::ObjectPositions;
    bindings[2].descriptorType      = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[2].descriptorCount     = 1;
    bindings[2].stageFlags          = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[3].binding             = VoxelPipeline::EShaderResource::ObjectRotations;
    bindings[3].descriptorType      = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[3].descriptorCount     = 1;
    bindings[3].stageFlags          = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[4].binding             = VoxelPipeline::EShaderResource::ObjectHalfSizes;
    bindings[4].descriptorType      = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[4].descriptorCount     = 1;
    bindings[4].stageFlags          = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo = { };
    layoutCreateInfo.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount   = static_cast<uint32_t>(bindings.size());
    layoutCreateInfo.pBindings      = &bindings[0];

    THROW_IF_FAILED(vkCreateDescriptorSetLayout(da->GetAdapterHandle(), 
                                                &layoutCreateInfo,
                                                NULL,
                                                &descriptorSetLayout));

    return descriptorSetLayout;
}

// ---------------------------------------------------------------------------------------------------------------------
VkDescriptorPool VoxelPipeline::CreateDescriptorPool(shared_ptr<const AdapterWrapper>& da)
{
    const vector<VkDescriptorPoolSize> poolSizes = {
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,  1 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 4 },
    };

    VkDescriptorPool descriptorPool;

    VkDescriptorPoolCreateInfo poolInfo = { };
    poolInfo.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount  = poolSizes.size();
    poolInfo.pPoolSizes     = &poolSizes[0];
    poolInfo.maxSets        = 1;

    THROW_IF_FAILED(vkCreateDescriptorPool(da->GetAdapterHandle(), &poolInfo, NULL, &descriptorPool));

    return descriptorPool;
}

// ---------------------------------------------------------------------------------------------------------------------
VkDescriptorSet VoxelPipeline::CreateDescriptorSet(shared_ptr<const AdapterWrapper>& da,
                                                   VkDescriptorPool descPool,
                                                   VkDescriptorSetLayout descLayout)
{
    VkDescriptorSet descriptorSet;

    VkDescriptorSetAllocateInfo allocInfo = { };
    allocInfo.sType                 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool        = descPool;
    allocInfo.descriptorSetCount    = 1;
    allocInfo.pSetLayouts           = &descLayout;

    THROW_IF_FAILED(vkAllocateDescriptorSets(da->GetAdapterHandle(), &allocInfo, &descriptorSet));

    return descriptorSet;
}

// ---------------------------------------------------------------------------------------------------------------------
VkPipelineLayout VoxelPipeline::CreatePipelineLayout(shared_ptr<const AdapterWrapper>& da,
                                                     VkDescriptorSetLayout descriptorSetLayout)
{ 
    VkPushConstantRange         pushConstantRange;
    VkPipelineLayoutCreateInfo  layoutInfo;
    VkPipelineLayout            pipelineLayout;

    pushConstantRange.stageFlags    = VK_SHADER_STAGE_COMPUTE_BIT;
    pushConstantRange.offset        = 0;
    pushConstantRange.size          = sizeof(VoxelPushConstants);

    layoutInfo.sType                    = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.pNext                    = NULL;
    layoutInfo.flags                    = 0;
    layoutInfo.setLayoutCount           = 1;
    layoutInfo.pSetLayouts              = &descriptorSetLayout;
    layoutInfo.pushConstantRangeCount   = 1;
    layoutInfo.pPushConstantRanges      = &pushConstantRange;

    THROW_IF_FAILED(vkCreatePipelineLayout(da->GetAdapterHandle(), 
                                           &layoutInfo,
                                           NULL,
                                           &pipelineLayout));

    return pipelineLayout;
}

// ---------------------------------------------------------------------------------------------------------------------
VkShaderModule VoxelPipeline::LoadShader(shared_ptr<const AdapterWrapper>& da, const string& strPath)
{
    vector<char>    vBuffer;
    size_t          uFileSize;
    VkShaderModule  shaderModule;

    ifstream file(strPath, ios::ate | ios::binary);

    if (!file.is_open()) {
        AB_LOG(Core::Debug::Error, L"%s", strPath.c_str());
        throw AB_EXCEPT("Failed to open shader file!");
    }

    uFileSize = static_cast<size_t>(file.tellg());
    vBuffer.resize(uFileSize);
    
    file.seekg(0);
    file.read(&vBuffer[0], uFileSize);

    file.close();
    
    VkShaderModuleCreateInfo shaderCreateInfo = { };
    shaderCreateInfo.sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderCreateInfo.codeSize   = vBuffer.size(); // Size is in bytes, so it's okay
    shaderCreateInfo.pCode      = reinterpret_cast<const uint32_t*>(&vBuffer[0]);

    THROW_IF_FAILED(vkCreateShaderModule(da->GetAdapterHandle(), &shaderCreateInfo, NULL, &shaderModule));

    return shaderModule;
}

// ---------------------------------------------------------------------------------------------------------------------
VkPipeline VoxelPipeline::CreateComputePipeline(shared_ptr<const AdapterWrapper>& da, 
                                                VkPipelineLayout pipelineLayout, 
                                                VkShaderModule shaderModule)
{ 
    const VkDevice  device      = da->GetAdapterHandle();
    VkPipeline      pipeline    = VK_NULL_HANDLE;

    VkPipelineShaderStageCreateInfo shaderStage = { };
    shaderStage.sType                   = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.stage                   = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderStage.module                  = shaderModule;
    shaderStage.pName                   = "main";


    VkComputePipelineCreateInfo pipelineInfo = { };
    pipelineInfo.sType              = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage              = shaderStage;
    pipelineInfo.layout             = pipelineLayout;

    THROW_IF_FAILED(vkCreateComputePipelines(device,
                                             VK_NULL_HANDLE,
                                             1,
                                             &pipelineInfo,
                                             NULL,
                                             &pipeline));

    return pipeline;
}

} // !B33::Rendering

