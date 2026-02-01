#include "B33Rendering.hpp"

#include "Raycaster/Renderer.hpp"

#include "Raycaster/VoxelGrid.hpp"
#include "Vec3.hpp"
#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/ComputeAdapter.hpp"
#include "Vulkan/FrameResources.hpp"
#include "Vulkan/GPUStreamBuffer.hpp"
#include "Vulkan/Memory.hpp"
#include "Vulkan/MinimalHardware.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

using namespace ::std;
using namespace ::B33::Math;

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Initialize(shared_ptr<const WindowDesc> wd,
                          shared_ptr<IWorldGrid> vg) 
{
    AB_LOG(Core::Debug::Info, L"Initializing renderer!");

    m_pInstance         = make_shared<Instance>();
    m_pHardware         = make_shared<MinimalHardware>(m_pInstance);
    m_pDeviceAdapter    = make_shared<ComputeAdapter>(static_pointer_cast<HardwareWrapper>(m_pHardware));
    m_pMemory           = make_unique<Memory>(m_pHardware, m_pDeviceAdapter);
    m_pWindowDesc       = wd;
    m_pVoxelGrid        = vg;
    m_pPipeline         = make_shared<VoxelPipeline>(static_pointer_cast<HardwareWrapper>(m_pHardware),
                                                     static_pointer_cast<AdapterWrapper>(m_pDeviceAdapter));

    AB_LOG(Core::Debug::Info, L"Initializing command pool");
    m_CommandPool = CreateCommandPool(static_pointer_cast<AdapterWrapper>(m_pDeviceAdapter),
                                      m_pDeviceAdapter->GetQueueFamilyIndex());

    m_StageVoxelBuffer          = std::move(m_pMemory->ReserveStagingBuffer(vg->GetVoxelsSizeInBytes()));
    m_StagePositonsBuffer       = std::move(m_pMemory->ReserveStagingBuffer(m_pVoxelGrid->GetStoredObjects().GetPositions().capacity() * sizeof(Vec3)));
    m_StageRotationsBuffer      = std::move(m_pMemory->ReserveStagingBuffer(m_pVoxelGrid->GetStoredObjects().GetRotations().capacity() * sizeof(Vec3)));
    m_StageHalfSizesBuffer      = std::move(m_pMemory->ReserveStagingBuffer((/*FIXME: */(Cubes&)m_pVoxelGrid->GetStoredObjects()).GetHalfSizes().capacity() * sizeof(Vec3)));
    m_VoxelBuffer       = std::move(m_pMemory->ReserveGPUBuffer(vg->GetVoxelsSizeInBytes()));
    m_PositionsBuffer   = std::move(m_pMemory->ReserveGPUBuffer(m_pVoxelGrid->GetStoredObjects().GetPositions().capacity() * sizeof(Vec3)));
    m_RotationsBuffer   = std::move(m_pMemory->ReserveGPUBuffer(m_pVoxelGrid->GetStoredObjects().GetRotations().capacity() * sizeof(Vec3)));
    m_HalfSizesBuffer   = std::move(m_pMemory->ReserveGPUBuffer((/*FIXME: */(Cubes&)m_pVoxelGrid->GetStoredObjects()).GetHalfSizes().capacity() * sizeof(Vec3)));

    // Recreating swap chain also creates frame resources and initializes swap chain
    RecreateSwapChain();

    if (m_pCamera == nullptr)
        m_pCamera = make_shared<Camera>();
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Update(const float)
{
    if (m_pWindowDesc->LastEvent & EAbWindowEvents::ChangedBehavior) {
        RecreateSwapChain();
    }

    auto& frames = *m_vFrames.get();

    if (m_pVoxelGrid->ReuploadStatus() & EReupload::RequestStaging) 
    {
        m_pMemory->UploadOnStreamBuffer(m_pVoxelGrid->GetGrid().data(), 
                                        m_pVoxelGrid->GetVoxelsSizeInBytes(),
                                        m_pPipeline->GetUniformUploadDescriptor(m_StageVoxelBuffer, 
                                                                       VoxelPipeline::EShaderResource::VoxelGrid));

        m_pMemory->UploadOnStreamBuffer(m_pVoxelGrid->GetStoredObjects().GetPositions().data(),
                                        m_pVoxelGrid->GetStoredObjects().GetPositions().capacity() * sizeof(Vec3),
                                        m_pPipeline->GetUniformUploadDescriptor(m_StagePositonsBuffer, 
                                                                       VoxelPipeline::EShaderResource::ObjectPositions));

        m_pMemory->UploadOnStreamBuffer(m_pVoxelGrid->GetStoredObjects().GetRotations().data(),
                                        m_pVoxelGrid->GetStoredObjects().GetRotations().capacity() * sizeof(Vec3),
                                        m_pPipeline->GetUniformUploadDescriptor(m_StageRotationsBuffer, 
                                                                       VoxelPipeline::EShaderResource::ObjectRotations));

        m_pMemory->UploadOnStreamBuffer((/*FIXME: */(Cubes&)m_pVoxelGrid->GetStoredObjects()).GetHalfSizes().data(),
                                        (/*FIXME: */(Cubes&)m_pVoxelGrid->GetStoredObjects()).GetHalfSizes().capacity() * sizeof(Vec3),
                                        m_pPipeline->GetUniformUploadDescriptor(m_StageHalfSizesBuffer, 
                                                                       VoxelPipeline::EShaderResource::ObjectHalfSizes));
    }

    Vec3 rot = m_pCamera->GetRotation();
    Vec3 rotVec = Normalize(RotateY(RotateX(Vec3 { 0.f, 0.f, 1.f }, rot.x), rot.y));
    Vec3 cameraRight = Normalize(Cross(rotVec, Vec3 { 0.f, -1.f, 0.f }));
    Vec3 cameraUp = Cross(cameraRight, rotVec);

    m_pPipeline->LoadPushConstants(m_pCamera->GetFov() * AB_DEG_TO_RAD,
                                   m_pCamera->GetPosition(), 
                                   rotVec, 
                                   cameraRight,
                                   cameraUp,
                                   m_pVoxelGrid->GetGridWidth(),
                                   m_bDebugMode ? 1 : 0);
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Render()
{
    VkResult result;
    VkDevice device = m_pDeviceAdapter->GetAdapterHandle();
    uint32_t uImageIndex;
    Frame& frame = (*m_vFrames.get())[m_uCurrentFrame];

    THROW_IF_FAILED(vkWaitForFences(device, 1, &frame.InFlightFence, VK_TRUE, UINT64_MAX));
    THROW_IF_FAILED(vkResetFences(device, 1, &frame.InFlightFence));

    result = vkAcquireNextImageKHR(device, 
                                   m_pSwapChain->GetSwapChainHandle(), 
                                   UINT64_MAX,
                                   frame.ImageAvailable,
                                   VK_NULL_HANDLE,
                                   &uImageIndex);

    if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR) {
        RecreateSwapChain();
        return;
    }

    m_pPipeline->LoadImage(m_pSwapChain->GetImage(uImageIndex));

    RecordCommands(frame.CommandBuffer,
                   m_pPipeline,
                   uImageIndex);

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submitInfo = { };
    submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = &frame.ImageAvailable;
    submitInfo.pWaitDstStageMask    = waitStages;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &frame.CommandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = &frame.RenderFinished;

    THROW_IF_FAILED(vkQueueSubmit(m_pDeviceAdapter->GetQueueHandle(), 1, &submitInfo, frame.InFlightFence));

    VkPresentInfoKHR presentInfo = { };
    presentInfo.sType                   = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount      = 1;
    presentInfo.pWaitSemaphores         = &frame.RenderFinished;
    presentInfo.swapchainCount          = 1;
    presentInfo.pSwapchains             = &m_pSwapChain->GetSwapChainHandle();
    presentInfo.pImageIndices           = &uImageIndex;

    result = vkQueuePresentKHR(m_pDeviceAdapter->GetQueueHandle(), &presentInfo);
    if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR) {
        RecreateSwapChain();
        return;
    }

    m_uCurrentFrame = (++m_uCurrentFrame) % Frame::MAX_FRAMES_IN_FLIGHT;
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Destroy()
{
    AB_LOG(Core::Debug::Info, L"Destroying renderer");

    if (m_pDeviceAdapter != nullptr)
        vkDeviceWaitIdle(m_pDeviceAdapter->GetAdapterHandle());

    if (m_pDeviceAdapter != nullptr)
        DestroyFrameResources();

    if (m_CommandPool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(m_pDeviceAdapter->GetAdapterHandle(), m_CommandPool, nullptr);
        m_CommandPool = VK_NULL_HANDLE;
    }
    
    m_StageVoxelBuffer      = nullptr;
    m_StagePositonsBuffer   = nullptr;
    m_StageRotationsBuffer  = nullptr;
    m_StageHalfSizesBuffer  = nullptr;
    m_VoxelBuffer       = nullptr;
    m_PositionsBuffer   = nullptr;
    m_RotationsBuffer   = nullptr;
    m_HalfSizesBuffer   = nullptr;
    m_pPipeline         = nullptr;
    m_pSwapChain        = nullptr;
    m_pDeviceAdapter    = nullptr;
    m_pHardware         = nullptr;
    m_pInstance         = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
VkCommandPool Renderer::CreateCommandPool(shared_ptr<const AdapterWrapper> da, uint32_t uQueueFamily)
{
    VkCommandPool cmdPool;

    VkCommandPoolCreateInfo cmdPoolInfo = { };
    cmdPoolInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.flags               = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cmdPoolInfo.queueFamilyIndex    = uQueueFamily;

    THROW_IF_FAILED(vkCreateCommandPool(da->GetAdapterHandle(), &cmdPoolInfo, NULL, &cmdPool));

    return cmdPool;
}
 
// ---------------------------------------------------------------------------------------------------------------------
VkCommandBuffer Renderer::CreateCommandBuffer(shared_ptr<const AdapterWrapper> da, VkCommandPool cmdPool)
{
    VkCommandBuffer cmdBuffer;

    VkCommandBufferAllocateInfo allocInfo = { };
    allocInfo.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext                 = NULL;
    allocInfo.commandPool           = cmdPool;
    allocInfo.level                 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount    = 1;

    THROW_IF_FAILED(vkAllocateCommandBuffers(m_pDeviceAdapter->GetAdapterHandle(), &allocInfo, &cmdBuffer));
    THROW_IF_FAILED(vkResetCommandBuffer(cmdBuffer, 0));

    return cmdBuffer;
}

// ---------------------------------------------------------------------------------------------------------------------
Renderer::FramesArray Renderer::CreateFrameResources(const shared_ptr<const AdapterWrapper>& da,
                                                             const unique_ptr<Memory>& memory,
                                                             const shared_ptr<const IWorldGrid>& vg,
                                                             VkCommandPool cmdPool,
                                                             size_t uFrames)
{
    VkDevice device = da->GetAdapterHandle();
    FramesArray result;

    VkSemaphoreCreateInfo semaphoreInfo = { };
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = { };
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < result.size(); ++i) 
    {
        if (vkCreateSemaphore(device, &semaphoreInfo, NULL, &result[i].ImageAvailable) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, NULL, &result[i].RenderFinished) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, NULL, &result[i].InFlightFence)          != VK_SUCCESS)
        {
            throw AB_EXCEPT("Failed to create frame resources!");
        }

        result[i].CommandBuffer = CreateCommandBuffer(da, cmdPool);
    }
    
    m_StageVoxelBuffer->Reset();
    m_StagePositonsBuffer->Reset();
    m_StageRotationsBuffer->Reset();
    m_StageHalfSizesBuffer->Reset();
    m_pVoxelGrid->ForceUpload();

    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::RecordCommands(VkCommandBuffer& cmdBuff,
                              const shared_ptr<VoxelPipeline>& pipeline,
                              uint32_t uImageIndex)
{
    vkResetCommandBuffer(cmdBuff, 0);

    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = NULL;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    beginInfo.pInheritanceInfo = NULL;

    THROW_IF_FAILED(vkBeginCommandBuffer(cmdBuff, &beginInfo));
    vkCmdBindPipeline(cmdBuff, VK_PIPELINE_BIND_POINT_COMPUTE, m_pPipeline->GetPipelineHandle());

    VkImageMemoryBarrier barrier = { };
    barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.oldLayout           = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barrier.newLayout           = VK_IMAGE_LAYOUT_GENERAL;
    barrier.srcAccessMask       = 0;
    barrier.dstAccessMask       = VK_ACCESS_SHADER_WRITE_BIT;
    barrier.image               = m_pSwapChain->GetImage(uImageIndex);
    barrier.subresourceRange    = VkImageSubresourceRange { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    vkCmdPipelineBarrier(cmdBuff,
                         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                         VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                         0,
                         0, NULL,
                         0, NULL,
                         1, &barrier);

    RecordVoxelesCommands(cmdBuff, pipeline);

    VkImageMemoryBarrier presentBarrier = { };
    presentBarrier.sType            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    presentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    presentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    presentBarrier.oldLayout        = VK_IMAGE_LAYOUT_GENERAL;
    presentBarrier.newLayout        = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    presentBarrier.srcAccessMask    = VK_ACCESS_SHADER_WRITE_BIT;
    presentBarrier.dstAccessMask    = 0;
    presentBarrier.image            = m_pSwapChain->GetImage(uImageIndex);
    presentBarrier.subresourceRange = VkImageSubresourceRange { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    vkCmdPipelineBarrier(cmdBuff,
                         VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                         0,
                         0, NULL,
                         0, NULL,
                         1, &presentBarrier);

    THROW_IF_FAILED(vkEndCommandBuffer(cmdBuff));
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::RecordVoxelesCommands(VkCommandBuffer& cmdBuffer, const shared_ptr<VoxelPipeline>& pipeline)
{
    auto& frames = *m_vFrames.get();

    vkCmdBindDescriptorSets(cmdBuffer,
                            VK_PIPELINE_BIND_POINT_COMPUTE,
                            pipeline->GetLayoutHandle(),
                            0, 1,
                            &pipeline->GetDescriptorSet(),
                            0, NULL);

    vkCmdPushConstants(cmdBuffer,
                       pipeline->GetLayoutHandle(),
                       VK_SHADER_STAGE_COMPUTE_BIT,
                       0,
                       sizeof(VoxelPushConstants),
                       &pipeline->GetPushConstants());

    if (m_pVoxelGrid->ReuploadStatus() & EReupload::RequestGpuUpload) 
    {
        VkBufferCopy copyRegion = { };
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size      = m_StageVoxelBuffer->GetSizeInBytes();
        vkCmdCopyBuffer(cmdBuffer, 
                        m_StageVoxelBuffer->GetBufferHandle(), 
                        m_VoxelBuffer->GetBufferHandle(), 
                        1,
                        &copyRegion);

        copyRegion.size = m_pVoxelGrid->GetStoredObjects().GetPositions().size() * sizeof(Vec3);
        vkCmdCopyBuffer(cmdBuffer, 
                        m_StagePositonsBuffer->GetBufferHandle(), 
                        m_PositionsBuffer->GetBufferHandle(), 
                        1,
                        &copyRegion);

        copyRegion.size = m_pVoxelGrid->GetStoredObjects().GetRotations().size() * sizeof(Vec3);
        vkCmdCopyBuffer(cmdBuffer, 
                        m_StageRotationsBuffer->GetBufferHandle(), 
                        m_RotationsBuffer->GetBufferHandle(), 
                        1,
                        &copyRegion);

        copyRegion.size = (/*FIXME: */(Cubes&)m_pVoxelGrid->GetStoredObjects()).GetHalfSizes().size() * sizeof(Vec3);
        vkCmdCopyBuffer(cmdBuffer, 
                        m_StageHalfSizesBuffer->GetBufferHandle(), 
                        m_HalfSizesBuffer->GetBufferHandle(), 
                        1,
                        &copyRegion);

        VkMappedMemoryRange mmr = { };
        mmr.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mmr.memory = m_VoxelBuffer->GetMemoryHandle();
        mmr.offset = 0;
        mmr.size = VK_WHOLE_SIZE;
        VkMappedMemoryRange mmr2 = { };
        mmr2.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mmr2.memory = m_PositionsBuffer->GetMemoryHandle();
        mmr2.offset = 0;
        mmr2.size = VK_WHOLE_SIZE;
        VkMappedMemoryRange mmr3 = { };
        mmr3.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mmr3.memory = m_RotationsBuffer->GetMemoryHandle();
        mmr3.offset = 0;
        mmr3.size = VK_WHOLE_SIZE;
        VkMappedMemoryRange mmr4 = { };
        mmr4.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mmr4.memory = m_HalfSizesBuffer->GetMemoryHandle();
        mmr4.offset = 0;
        mmr4.size = VK_WHOLE_SIZE;

        VkMappedMemoryRange mmrs[4] = { mmr, mmr2, mmr3, mmr4 };
        vkFlushMappedMemoryRanges(m_pDeviceAdapter->GetAdapterHandle(), 4, mmrs);
    }

    VkBufferMemoryBarrier bufferBarriers[4] = { };
    bufferBarriers[0].sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    bufferBarriers[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    bufferBarriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    bufferBarriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferBarriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferBarriers[0].buffer = m_VoxelBuffer->GetBufferHandle();
    bufferBarriers[0].offset = 0;
    bufferBarriers[0].size = VK_WHOLE_SIZE;
    
    bufferBarriers[1] = bufferBarriers[0];
    bufferBarriers[1].buffer = m_PositionsBuffer->GetBufferHandle();
    
    bufferBarriers[2] = bufferBarriers[1];
    bufferBarriers[2].buffer = m_RotationsBuffer->GetBufferHandle();
    
    bufferBarriers[3] = bufferBarriers[2];
    bufferBarriers[3].buffer = m_HalfSizesBuffer->GetBufferHandle();

    vkCmdPipelineBarrier(cmdBuffer,
                         VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                         0,
                         0, NULL,
                         4, bufferBarriers,
                         0, NULL);

    const uint32_t groupCountX = (m_pWindowDesc->Width + 31) / 32;
    const uint32_t groupCountY = (m_pWindowDesc->Height + 7) / 8;
    vkCmdDispatch(cmdBuffer, groupCountX, groupCountY, 1);
}

// --------------------------------------------------------------------------------------------------------------------
void Renderer::DestroyFrameResources()
{
    AB_LOG(Core::Debug::Info, L"Destroying frame resources");

    if (m_vFrames != nullptr) 
    {
        auto& frames = *m_vFrames.get();

        for (size_t i = 0; i < frames.size(); ++i)
                vkWaitForFences(m_pDeviceAdapter->GetAdapterHandle(), 1, &frames[i].InFlightFence, VK_TRUE, UINT64_MAX);

        for (size_t i = 0; i < frames.size(); ++i)
        {
            vkDestroySemaphore(m_pDeviceAdapter->GetAdapterHandle(), frames[i].RenderFinished, nullptr);
            vkDestroySemaphore(m_pDeviceAdapter->GetAdapterHandle(), frames[i].ImageAvailable, nullptr);
            vkDestroyFence(m_pDeviceAdapter->GetAdapterHandle(), frames[i].InFlightFence, nullptr);
            vkFreeCommandBuffers(m_pDeviceAdapter->GetAdapterHandle(), m_CommandPool, 1, &frames[i].CommandBuffer);
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::RecreateSwapChain()
{
    AB_LOG(Core::Debug::Info, L"Recreating swapchain");
    if (m_pWindowDesc->bIsAlive == false) {
        return;
    }

    DestroyFrameResources();

    m_pSwapChain = nullptr;
    m_pSwapChain = make_unique<Swapchain>(m_pInstance, 
                                          static_pointer_cast<HardwareWrapper>(m_pHardware), 
                                          static_pointer_cast<AdapterWrapper>(m_pDeviceAdapter),
                                          m_pWindowDesc);

    m_vFrames = make_unique<FramesArray>(std::move(CreateFrameResources(m_pDeviceAdapter,
                                                                        m_pMemory,
                                                                        m_pVoxelGrid,
                                                                        m_CommandPool,
                                                                        Frame::MAX_FRAMES_IN_FLIGHT)));
    m_uCurrentFrame = 0;
    AB_LOG(Core::Debug::Info, L"Swapchain recreated");
}

} // !Voxels
