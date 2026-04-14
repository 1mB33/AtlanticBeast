#include "B33Rendering.hpp"

#include "Editor/EditorPipeline.hpp"
#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/GPUStreamBuffer.hpp"
#include "Vulkan/Memory.hpp"
#include "Vulkan/SwapChain.hpp"

namespace B33::Rendering
{

using namespace ::std;
using namespace ::B33::Math;

// ---------------------------------------------------------------------------------------------------------------------
EditorPipeline::~EditorPipeline()
{
    B33_LOG( Core::Debug::Info, L"Destroying pipeline" );

    if ( m_ImageView != VK_NULL_HANDLE )
    {
        vkDestroyImageView( GetAdaterInternal()->GetAdapterHandle(), m_ImageView, nullptr );
        m_ImageView = VK_NULL_HANDLE;
    }
}

// Public // -----------------------------------------------------------------------------------------------------------
void EditorPipeline::CreatePipelineResourcesImpl() {}

// --------------------------------------------------------------------------------------------------------------------
void EditorPipeline::Update() {}

// --------------------------------------------------------------------------------------------------------------------
void EditorPipeline::RecordCommands( VkPipelineStageFlagBits lastStage,
                                     VkCommandBuffer        &cmdBuffer,
                                     uint32_t                uImageIndex )
{
    vkCmdPushConstants( cmdBuffer,
                        GetLayoutHandle(),
                        VK_SHADER_STAGE_COMPUTE_BIT,
                        0,
                        sizeof( EditorPushConstants ),
                        &m_Vpc );

    vkCmdPipelineBarrier( cmdBuffer, lastStage, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, NULL, 0, NULL, 0, NULL );

    const uint32_t groupCountX = ( m_pWindowDesc->Width + 31 ) >> 5;
    const uint32_t groupCountY = ( m_pWindowDesc->Height + 7 ) >> 3;
    vkCmdDispatch( cmdBuffer, groupCountX, groupCountY, 1 );
}

// --------------------------------------------------------------------------------------------------------------------
void EditorPipeline::Reset() {}

// ---------------------------------------------------------------------------------------------------------------------
void EditorPipeline::LoadImage( VkImage image )
{
    if ( m_ImageView != VK_NULL_HANDLE )
    {
        vkDestroyImageView( GetAdaterInternal()->GetAdapterHandle(), m_ImageView, nullptr );
        m_ImageView = VK_NULL_HANDLE;
    }

    VkImageViewCreateInfo viewInfo           = {};
    viewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image                           = image;
    viewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format                          = Swapchain::TargetedFormat;
    viewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel   = 0;
    viewInfo.subresourceRange.levelCount     = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount     = 1;

    THROW_IF_FAILED( vkCreateImageView( GetAdaterInternal()->GetAdapterHandle(), &viewInfo, NULL, &m_ImageView ) );

    VkDescriptorImageInfo imageInfo = {};
    imageInfo.imageView             = m_ImageView;
    imageInfo.imageLayout           = VK_IMAGE_LAYOUT_GENERAL;

    VkWriteDescriptorSet imageWrite = {};
    imageWrite.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    imageWrite.dstSet               = this->GetDescriptorSet();
    imageWrite.dstBinding           = 0;
    imageWrite.dstArrayElement      = 0;
    imageWrite.descriptorType       = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    imageWrite.descriptorCount      = 1;
    imageWrite.pImageInfo           = &imageInfo;

    vkUpdateDescriptorSets( GetAdaterInternal()->GetAdapterHandle(), 1, &imageWrite, 0, NULL );
}

// Private // ----------------------------------------------------------------------------------------------------------
VkDescriptorSetLayout EditorPipeline::CreateDescriptorLayoutImpl()
{
    array<VkDescriptorSetLayoutBinding, 1> bindings = {};
    VkDescriptorSetLayout                  descriptorSetLayout;

    bindings[ 0 ].binding         = 0;
    bindings[ 0 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    bindings[ 0 ].descriptorCount = 1;
    bindings[ 0 ].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {};
    layoutCreateInfo.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount                    = static_cast<uint32_t>( bindings.size() );
    layoutCreateInfo.pBindings                       = &bindings[ 0 ];

    THROW_IF_FAILED( vkCreateDescriptorSetLayout( GetAdaterInternal()->GetAdapterHandle(),
                                                  &layoutCreateInfo,
                                                  NULL,
                                                  &descriptorSetLayout ) );

    return descriptorSetLayout;
}

// ---------------------------------------------------------------------------------------------------------------------
VkDescriptorPool EditorPipeline::CreateDescriptorPoolImpl()
{
    const vector<VkDescriptorPoolSize> poolSizes = {
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1 },
    };

    VkDescriptorPool descriptorPool;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount              = poolSizes.size();
    poolInfo.pPoolSizes                 = &poolSizes[ 0 ];
    poolInfo.maxSets                    = 1;

    THROW_IF_FAILED(
        vkCreateDescriptorPool( GetAdaterInternal()->GetAdapterHandle(), &poolInfo, NULL, &descriptorPool ) );

    return descriptorPool;
}

// ---------------------------------------------------------------------------------------------------------------------
VkDescriptorSet EditorPipeline::CreateDescriptorSetImpl()
{
    VkDescriptorSetLayout descLayout = GetDescriptorLayoutInternal();
    VkDescriptorSet       descriptorSet;

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType                       = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool              = GetDescriptorPoolInternal();
    allocInfo.descriptorSetCount          = 1;
    allocInfo.pSetLayouts                 = &descLayout;

    THROW_IF_FAILED( vkAllocateDescriptorSets( GetAdaterInternal()->GetAdapterHandle(), &allocInfo, &descriptorSet ) );

    return descriptorSet;
}

// ---------------------------------------------------------------------------------------------------------------------
VkPipelineLayout EditorPipeline::CreatePipelineLayoutImpl()
{
    VkDescriptorSetLayout      descLayout = GetDescriptorLayoutInternal();
    VkPushConstantRange        pushConstantRange;
    VkPipelineLayoutCreateInfo layoutInfo;
    VkPipelineLayout           pipelineLayout;

    pushConstantRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    pushConstantRange.offset     = 0;
    pushConstantRange.size       = sizeof( EditorPushConstants );

    layoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.pNext                  = NULL;
    layoutInfo.flags                  = 0;
    layoutInfo.setLayoutCount         = 1;
    layoutInfo.pSetLayouts            = &descLayout;
    layoutInfo.pushConstantRangeCount = 1;
    layoutInfo.pPushConstantRanges    = &pushConstantRange;

    THROW_IF_FAILED(
        vkCreatePipelineLayout( GetAdaterInternal()->GetAdapterHandle(), &layoutInfo, NULL, &pipelineLayout ) );

    return pipelineLayout;
}

// ---------------------------------------------------------------------------------------------------------------------
VkShaderModule EditorPipeline::LoadShaderImpl( const string &strPath )
{
    vector<char>   vBuffer;
    size_t         uFileSize;
    VkShaderModule shaderModule;

    ifstream file( strPath, ios::ate | ios::binary );

    if ( !file.is_open() )
    {
        B33_LOG( Core::Debug::Error, L"%s", strPath.c_str() );
        throw B33_EXCEPT( "Failed to open shader file!" );
    }

    uFileSize = static_cast<size_t>( file.tellg() );
    vBuffer.resize( uFileSize );

    file.seekg( 0 );
    file.read( &vBuffer[ 0 ], uFileSize );

    file.close();

    VkShaderModuleCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderCreateInfo.codeSize                 = vBuffer.size(); // Size is in bytes, so it's okay
    shaderCreateInfo.pCode                    = reinterpret_cast<const uint32_t *>( &vBuffer[ 0 ] );

    THROW_IF_FAILED(
        vkCreateShaderModule( GetAdaterInternal()->GetAdapterHandle(), &shaderCreateInfo, NULL, &shaderModule ) );

    return shaderModule;
}

// ---------------------------------------------------------------------------------------------------------------------
VkPipeline EditorPipeline::CreatePipelineImpl()
{
    const VkDevice device   = GetAdaterInternal()->GetAdapterHandle();
    VkPipeline     pipeline = VK_NULL_HANDLE;

    VkPipelineShaderStageCreateInfo shaderStage = {};
    shaderStage.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.stage                           = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderStage.module                          = GetShaderModuleInternal();
    shaderStage.pName                           = "main";

    VkComputePipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType                       = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage                       = shaderStage;
    pipelineInfo.layout                      = this->GetLayoutHandle();

    THROW_IF_FAILED( vkCreateComputePipelines( device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &pipeline ) );

    return pipeline;
}

} // namespace B33::Rendering
