#include "B33Core.h"
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
VoxelPipeline::~VoxelPipeline()
{
    B33_LOG( Core::Debug::Info, L"Destroying pipeline" );

    if ( m_ImageView != VK_NULL_HANDLE )
    {
        vkDestroyImageView( GetAdaterInternal()->GetAdapterHandle(), m_ImageView, nullptr );
        m_ImageView = VK_NULL_HANDLE;
    }

    m_StageVoxelBuffer     = nullptr;
    m_StagePositonsBuffer  = nullptr;
    m_StageRotationsBuffer = nullptr;
    m_StageHalfSizesBuffer = nullptr;
    m_VoxelBuffer          = nullptr;
    m_PositionsBuffer      = nullptr;
    m_RotationsBuffer      = nullptr;
    m_HalfSizesBuffer      = nullptr;
}

// Public // -----------------------------------------------------------------------------------------------------------
void VoxelPipeline::CreatePipelineResourcesImpl( ::std::shared_ptr<::B33::Rendering::CubeWorld> pWorld )
{
    m_pVoxelGrid = pWorld;

    m_StageVoxelBuffer = std::move( GetMemoryInternal()->ReserveStagingBuffer( m_pVoxelGrid->GetVoxelsSizeInBytes() ) );
    m_StagePositonsBuffer  = std::move( GetMemoryInternal()->ReserveStagingBuffer(
        m_pVoxelGrid->GetStoredObjects().GetPositions().capacity() * sizeof( Vec3 ) ) );
    m_StageRotationsBuffer = std::move( GetMemoryInternal()->ReserveStagingBuffer(
        m_pVoxelGrid->GetStoredObjects().GetRotations().capacity() * sizeof( Vec3 ) ) );
    m_StageHalfSizesBuffer = std::move( GetMemoryInternal()->ReserveStagingBuffer(
        ( /*FIXME: */ (Cubes &)m_pVoxelGrid->GetStoredObjects() ).GetHalfSizes().capacity() * sizeof( Vec3 ) ) );
    m_VoxelBuffer          = std::move( GetMemoryInternal()->ReserveGPUBuffer( m_pVoxelGrid->GetVoxelsSizeInBytes() ) );
    m_PositionsBuffer      = std::move( GetMemoryInternal()->ReserveGPUBuffer(
        m_pVoxelGrid->GetStoredObjects().GetPositions().capacity() * sizeof( Vec3 ) ) );
    m_RotationsBuffer      = std::move( GetMemoryInternal()->ReserveGPUBuffer(
        m_pVoxelGrid->GetStoredObjects().GetRotations().capacity() * sizeof( Vec3 ) ) );
    m_HalfSizesBuffer      = std::move( GetMemoryInternal()->ReserveGPUBuffer(
        ( /*FIXME: */ (Cubes &)m_pVoxelGrid->GetStoredObjects() ).GetHalfSizes().capacity() * sizeof( Vec3 ) ) );
}

// --------------------------------------------------------------------------------------------------------------------
void VoxelPipeline::Update()
{
    auto swapChain = GetSwapChainInternal().lock();
    if ( swapChain == nullptr )
    {
        B33_ERROR( L"Swapchain unavilible for pipeline" );
        return;
    }

    this->LoadImage( swapChain->GetImage() );

    if ( !( m_pVoxelGrid->ReuploadStatus() & EReupload::RequestStaging ) )
        return;

    m_uStorageBuffersFlags = m_pVoxelGrid->GetChanged();

    GetMemoryInternal()->UploadOnStreamBuffer(
        m_pVoxelGrid->GetGrid().data(),
        m_pVoxelGrid->GetGrid().size() * sizeof( Voxel ),
        GetUniformUploadDescriptor( m_StageVoxelBuffer, VoxelPipeline::EShaderResource::VoxelGrid ) );

    if ( m_uStorageBuffersFlags & EGridChanged::Position )
    {
        GetMemoryInternal()->UploadOnStreamBuffer(
            m_pVoxelGrid->GetStoredObjects().GetPositions().data(),
            m_pVoxelGrid->GetStoredObjects().GetPositions().size() * sizeof( Vec3 ),
            GetUniformUploadDescriptor( m_StagePositonsBuffer, VoxelPipeline::EShaderResource::ObjectPositions ) );
    }

    if ( m_uStorageBuffersFlags & EGridChanged::Rotation )
    {
        GetMemoryInternal()->UploadOnStreamBuffer(
            m_pVoxelGrid->GetStoredObjects().GetRotations().data(),
            m_pVoxelGrid->GetStoredObjects().GetRotations().size() * sizeof( Vec3 ),
            GetUniformUploadDescriptor( m_StageRotationsBuffer, VoxelPipeline::EShaderResource::ObjectRotations ) );
    }

    if ( m_uStorageBuffersFlags & EGridChanged::HalfSize )
    {
        GetMemoryInternal()->UploadOnStreamBuffer(
            ( static_cast<const Cubes &>( m_pVoxelGrid->GetStoredObjects() ) ).GetHalfSizes().data(),
            ( static_cast<const Cubes &>( m_pVoxelGrid->GetStoredObjects() ) ).GetHalfSizes().size() * sizeof( Vec3 ),
            GetUniformUploadDescriptor( m_StageHalfSizesBuffer, VoxelPipeline::EShaderResource::ObjectHalfSizes ) );
    }
}

// --------------------------------------------------------------------------------------------------------------------
void VoxelPipeline::RecordCommands( VkPipelineStageFlagBits lastStage, VkCommandBuffer &cmdBuffer )
{
    vkCmdPushConstants( cmdBuffer,
                        GetLayoutHandle(),
                        VK_SHADER_STAGE_COMPUTE_BIT,
                        0,
                        sizeof( VoxelPushConstants ),
                        &m_Vpc );

    if ( m_pVoxelGrid->ReuploadStatus() & EReupload::RequestGpuUpload )
    {
        VkBufferCopy copyRegion = {};
        copyRegion.srcOffset    = 0;
        copyRegion.dstOffset    = 0;
        copyRegion.size         = m_StageVoxelBuffer->GetSizeInBytes();
        vkCmdCopyBuffer( cmdBuffer,
                         m_StageVoxelBuffer->GetBufferHandle(),
                         m_VoxelBuffer->GetBufferHandle(),
                         1,
                         &copyRegion );

        if ( m_uStorageBuffersFlags & EGridChanged::Position )
        {
            copyRegion.size = m_pVoxelGrid->GetStoredObjects().GetPositions().size() * sizeof( Vec3 );
            vkCmdCopyBuffer( cmdBuffer,
                             m_StagePositonsBuffer->GetBufferHandle(),
                             m_PositionsBuffer->GetBufferHandle(),
                             1,
                             &copyRegion );
        }

        if ( m_uStorageBuffersFlags & EGridChanged::Rotation )
        {
            copyRegion.size = m_pVoxelGrid->GetStoredObjects().GetRotations().size() * sizeof( Vec3 );
            vkCmdCopyBuffer( cmdBuffer,
                             m_StageRotationsBuffer->GetBufferHandle(),
                             m_RotationsBuffer->GetBufferHandle(),
                             1,
                             &copyRegion );
        }

        if ( m_uStorageBuffersFlags & EGridChanged::HalfSize )
        {
            copyRegion.size = ( static_cast<const Cubes &>( m_pVoxelGrid->GetStoredObjects() ) ).GetHalfSizes().size() *
                              sizeof( Vec3 );
            vkCmdCopyBuffer( cmdBuffer,
                             m_StageHalfSizesBuffer->GetBufferHandle(),
                             m_HalfSizesBuffer->GetBufferHandle(),
                             1,
                             &copyRegion );
        }

        vector<VkMappedMemoryRange> mmrs = {};

        VkMappedMemoryRange mmr = {};
        mmr.sType               = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mmr.memory              = m_VoxelBuffer->GetMemoryHandle();
        mmr.offset              = 0;
        mmr.size                = VK_WHOLE_SIZE;
        mmrs.push_back( mmr );

        if ( m_uStorageBuffersFlags & EGridChanged::Position )
        {
            VkMappedMemoryRange mmr2 = mmr;
            mmr2.memory              = m_PositionsBuffer->GetMemoryHandle();
            mmrs.push_back( mmr2 );
        }
        if ( m_uStorageBuffersFlags & EGridChanged::Rotation )
        {
            VkMappedMemoryRange mmr3 = mmr;
            mmr3.memory              = m_RotationsBuffer->GetMemoryHandle();
            mmrs.push_back( mmr3 );
        }
        if ( m_uStorageBuffersFlags & EGridChanged::HalfSize )
        {
            VkMappedMemoryRange mmr4 = mmr;
            mmr4.memory              = m_HalfSizesBuffer->GetMemoryHandle();
            mmrs.push_back( mmr4 );
        }

        vkFlushMappedMemoryRanges( GetAdaterInternal()->GetAdapterHandle(), mmrs.size(), mmrs.data() );
        m_uStorageBuffersFlags &= 0;
    }

    VkBufferMemoryBarrier bufferBarriers[ 4 ] = {};
    bufferBarriers[ 0 ].sType                 = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    bufferBarriers[ 0 ].srcAccessMask         = VK_ACCESS_TRANSFER_WRITE_BIT;
    bufferBarriers[ 0 ].dstAccessMask         = VK_ACCESS_SHADER_READ_BIT;
    bufferBarriers[ 0 ].srcQueueFamilyIndex   = VK_QUEUE_FAMILY_IGNORED;
    bufferBarriers[ 0 ].dstQueueFamilyIndex   = VK_QUEUE_FAMILY_IGNORED;
    bufferBarriers[ 0 ].buffer                = m_VoxelBuffer->GetBufferHandle();
    bufferBarriers[ 0 ].offset                = 0;
    bufferBarriers[ 0 ].size                  = VK_WHOLE_SIZE;

    bufferBarriers[ 1 ]        = bufferBarriers[ 0 ];
    bufferBarriers[ 1 ].buffer = m_PositionsBuffer->GetBufferHandle();

    bufferBarriers[ 2 ]        = bufferBarriers[ 1 ];
    bufferBarriers[ 2 ].buffer = m_RotationsBuffer->GetBufferHandle();

    bufferBarriers[ 3 ]        = bufferBarriers[ 2 ];
    bufferBarriers[ 3 ].buffer = m_HalfSizesBuffer->GetBufferHandle();

    vkCmdPipelineBarrier( cmdBuffer,
                          lastStage,
                          VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                          0,
                          0,
                          NULL,
                          4,
                          bufferBarriers,
                          0,
                          NULL );

    const uint32_t groupCountX = ( GetWindowDescInternal()->Width + 31 ) >> 5;
    const uint32_t groupCountY = ( GetWindowDescInternal()->Height + 7 ) >> 3;
    vkCmdDispatch( cmdBuffer, groupCountX, groupCountY, 1 );
}

// --------------------------------------------------------------------------------------------------------------------
void VoxelPipeline::Reset()
{
    if ( m_StageVoxelBuffer == nullptr || m_StagePositonsBuffer == nullptr || m_StageHalfSizesBuffer == nullptr ||
         m_pVoxelGrid == nullptr )
    {
        return;
    }

    m_StageVoxelBuffer->Reset();
    m_StagePositonsBuffer->Reset();
    m_StageRotationsBuffer->Reset();
    m_StageHalfSizesBuffer->Reset();
    m_pVoxelGrid->ForceUpload();
}

// --------------------------------------------------------------------------------------------------------------------
UploadDescriptor VoxelPipeline::GetUniformUploadDescriptor( const shared_ptr<GPUStreamBuffer> &outBuffer,
                                                            const EShaderResource             &sr )
{
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer                 = outBuffer->GetBufferHandle();
    bufferInfo.offset                 = 0;
    bufferInfo.range                  = VK_WHOLE_SIZE;

    VkWriteDescriptorSet write = {};
    write.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet               = GetDescriptorSet();
    write.dstBinding           = sr;
    write.descriptorType       = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    write.descriptorCount      = 1;
    write.pBufferInfo          = &bufferInfo;

    return UploadDescriptor( std::move( bufferInfo ),
                             std::move( write ),
                             UploadDescriptor::EUploadType::StreamBuffer,
                             outBuffer );
}

// ---------------------------------------------------------------------------------------------------------------------
void VoxelPipeline::LoadImage( VkImage image )
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
VkDescriptorSetLayout VoxelPipeline::CreateDescriptorLayoutImpl()
{
    array<VkDescriptorSetLayoutBinding, 5> bindings = {};
    VkDescriptorSetLayout                  descriptorSetLayout;

    bindings[ 0 ].binding         = 0;
    bindings[ 0 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    bindings[ 0 ].descriptorCount = 1;
    bindings[ 0 ].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[ 1 ].binding         = VoxelPipeline::EShaderResource::VoxelGrid;
    bindings[ 1 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[ 1 ].descriptorCount = 1;
    bindings[ 1 ].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[ 2 ].binding         = VoxelPipeline::EShaderResource::ObjectPositions;
    bindings[ 2 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[ 2 ].descriptorCount = 1;
    bindings[ 2 ].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[ 3 ].binding         = VoxelPipeline::EShaderResource::ObjectRotations;
    bindings[ 3 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[ 3 ].descriptorCount = 1;
    bindings[ 3 ].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[ 4 ].binding         = VoxelPipeline::EShaderResource::ObjectHalfSizes;
    bindings[ 4 ].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[ 4 ].descriptorCount = 1;
    bindings[ 4 ].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

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
VkDescriptorPool VoxelPipeline::CreateDescriptorPoolImpl()
{
    const vector<VkDescriptorPoolSize> poolSizes = {
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 4 },
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
VkDescriptorSet VoxelPipeline::CreateDescriptorSetImpl()
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
VkPipelineLayout VoxelPipeline::CreatePipelineLayoutImpl()
{
    VkDescriptorSetLayout      descLayout = GetDescriptorLayoutInternal();
    VkPushConstantRange        pushConstantRange;
    VkPipelineLayoutCreateInfo layoutInfo;
    VkPipelineLayout           pipelineLayout;

    pushConstantRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    pushConstantRange.offset     = 0;
    pushConstantRange.size       = sizeof( VoxelPushConstants );

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
VkShaderModule VoxelPipeline::LoadShaderImpl( const string &strPath )
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
VkPipeline VoxelPipeline::CreatePipelineImpl()
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
