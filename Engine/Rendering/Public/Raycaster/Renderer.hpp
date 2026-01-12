#ifndef AB_RENDERER_H
#define AB_RENDERER_H

#include "Vulkan/FrameResources.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/MinimalHardware.hpp"
#include "Vulkan/ComputeAdapter.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Raycaster/VoxelPipeline.hpp"
#include "Primitives/Camera.hpp"

namespace Voxels
{

class Renderer
{
    using FrameResourcesArray = ::std::array<FrameResources, Voxels::FrameResources::MAX_FRAMES_IN_FLIGHT>;

public:

    Renderer()
        : m_pInstance(nullptr)
        , m_pHardware(nullptr)
        , m_pDeviceAdapter(nullptr)
        , m_pWindowDesc(nullptr)
        , m_pSwapChain(nullptr)
        , m_pMemory(nullptr)
        , m_pPipeline(nullptr)
        , m_pVoxelGrid(nullptr)
        , m_CommandPool(VK_NULL_HANDLE)
        , m_uCurrentFrame(0)
        , m_vFrames()
        , m_bDebugMode(false)
    { }

    ~Renderer()
    { Destroy();}

public:

    void SetCurrentCamera(::std::shared_ptr<Camera> camera)
    { m_pCamera = camera; }

    void SetDebugMode(const bool bMode)
    { m_bDebugMode = bMode; }

public:

    ::std::shared_ptr<Camera>& GetCurrentCamera()
    {
        if (m_pCamera == nullptr) {
            AB_LOG(Core::Debug::Warning, L"Renderer doesn't have a camera. Recreating camera for getter.");
            m_pCamera = ::std::make_shared<Camera>();
        }

        return m_pCamera;
    }

    ::std::shared_ptr<IWorldGrid>& GetGrid()
    { return m_pVoxelGrid; }

    bool GetDebugMode()
    { return m_bDebugMode; }

public:

    BEAST_API void Initialize(::std::shared_ptr<const WindowDesc> wd,
                              ::std::shared_ptr<IWorldGrid> vg);

    BEAST_API void Update(const float);

    BEAST_API void Render();

    BEAST_API void Destroy();

private:

    VkCommandPool CreateCommandPool(::std::shared_ptr<const AdapterWrapper> da,
                                    uint32_t uQueueFamily);

    VkCommandBuffer CreateCommandBuffer(::std::shared_ptr<const AdapterWrapper> da,
                                        VkCommandPool cmdPool);

    FrameResourcesArray CreateFrameResources(const ::std::shared_ptr<const AdapterWrapper>& da,
                                             const ::std::unique_ptr<Memory>& memory,
                                             const ::std::shared_ptr<const IWorldGrid>& vg,
                                             VkCommandPool cmdPool,
                                             size_t uFrames);

    void RecordCommands(VkCommandBuffer& cmdBuff,
                        const ::std::shared_ptr<VoxelPipeline>& pipeline, 
                        uint32_t uImageIndex);

    void RecordVoxelesCommands(VkCommandBuffer& cmdBuffer,
                               const ::std::shared_ptr<VoxelPipeline>& pipeline);

private:

    void DestroyFrameResources();

    void RecreateSwapChain();

private:

    ::std::shared_ptr<Instance>             m_pInstance         = nullptr;
    ::std::shared_ptr<MinimalHardware>      m_pHardware         = nullptr;
    ::std::shared_ptr<ComputeAdapter>       m_pDeviceAdapter    = nullptr;
    ::std::shared_ptr<const WindowDesc>     m_pWindowDesc       = nullptr;
    ::std::shared_ptr<IWorldGrid>           m_pVoxelGrid        = nullptr;
    ::std::shared_ptr<Camera>               m_pCamera           = nullptr;
    ::std::unique_ptr<Swapchain>            m_pSwapChain        = nullptr;
    ::std::unique_ptr<Memory>               m_pMemory           = nullptr;
    ::std::shared_ptr<VoxelPipeline>        m_pPipeline         = nullptr;

    VkCommandPool m_CommandPool = VK_NULL_HANDLE;

    size_t m_uCurrentFrame;
    ::std::unique_ptr<FrameResourcesArray> m_vFrames = nullptr;

    ::std::shared_ptr<GPUBuffer>    m_VoxelBuffer;
    ::std::shared_ptr<GPUBuffer>    m_CubeBuffer;    
    ::std::shared_ptr<GPUStreamBuffer> m_StageVoxelBuffer;
    ::std::shared_ptr<GPUStreamBuffer> m_StageCubeBuffer;

    bool m_bDebugMode;
};

} // !Voxels

#endif // !AB_RENDERER_H
