#ifndef AB_RENDERER_H
#define AB_RENDERER_H

#include "Vulkan/FrameResources.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/MinimalHardware.hpp"
#include "Vulkan/ComputeAdapter.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Raycaster/VoxelGrid.hpp"
#include "Raycaster/VoxelPipeline.hpp"
#include "Primitives/Camera.hpp"

namespace B33::Rendering
{

class Renderer
{
    using FramesArray = ::std::array<::B33::Rendering::Frame, 
                                     ::B33::Rendering::Frame::MAX_FRAMES_IN_FLIGHT>;

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
        , m_uStorageBuffersFlags(0)
        , m_bDebugMode(false)
    { }

    ~Renderer()
    { Destroy(); }

public:

    ::std::shared_ptr<::B33::Rendering::Camera>& GetCurrentCamera()
    {
        if (m_pCamera == nullptr) {
            AB_LOG(::B33::Core::Debug::Warning, L"Renderer doesn't have a camera. Recreating default camera on fly.");
            m_pCamera = ::std::make_shared<::B33::Rendering::Camera>();
        }

        return m_pCamera;
    }

    ::std::shared_ptr<::B33::Rendering::IWorldGrid>& GetGrid()
    { return m_pVoxelGrid; }

    bool GetDebugMode()
    { return m_bDebugMode; }

public:

    void SetCurrentCamera(::std::shared_ptr<::B33::Rendering::Camera> camera)
    { m_pCamera = camera; }

    void SetDebugMode(const bool bMode)
    { m_bDebugMode = bMode; }

public:

    BEAST_API void Initialize(::std::shared_ptr<const ::WindowDesc> wd,
                              ::std::shared_ptr<::B33::Rendering::IWorldGrid> vg);

    BEAST_API void Update(const float fDelta);

    BEAST_API void Render();

    BEAST_API void Destroy();

private:

    ::VkCommandPool CreateCommandPool(::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da,
                                      ::uint32_t uQueueFamily);

    ::VkCommandBuffer CreateCommandBuffer(::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da,
                                        ::VkCommandPool cmdPool);

    FramesArray CreateFrameResources(const ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>& da,
                                     const ::std::unique_ptr<::B33::Rendering::Memory>& memory,
                                     const ::std::shared_ptr<const ::B33::Rendering::IWorldGrid>& vg,
                                     ::VkCommandPool cmdPool,
                                     ::size_t uFrames);

    void RecordCommands(::VkCommandBuffer& cmdBuff,
                        const ::std::shared_ptr<::B33::Rendering::VoxelPipeline>& pipeline, 
                        ::uint32_t uImageIndex);

    void RecordVoxelesCommands(::VkCommandBuffer& cmdBuffer,
                               const ::std::shared_ptr<::B33::Rendering::VoxelPipeline>& pipeline);

private:

    void DestroyFrameResources();

    void RecreateSwapChain();

private:

    ::std::shared_ptr<::B33::Rendering::Instance>               m_pInstance         = nullptr;
    ::std::shared_ptr<::B33::Rendering::MinimalHardware>        m_pHardware         = nullptr;
    ::std::shared_ptr<::B33::Rendering::ComputeAdapter>         m_pDeviceAdapter    = nullptr;
    ::std::shared_ptr<const ::WindowDesc>                       m_pWindowDesc       = nullptr;
    ::std::shared_ptr<::B33::Rendering::IWorldGrid>             m_pVoxelGrid        = nullptr;
    ::std::shared_ptr<::B33::Rendering::Camera>                 m_pCamera           = nullptr;
    ::std::unique_ptr<::B33::Rendering::Swapchain>              m_pSwapChain        = nullptr;
    ::std::unique_ptr<::B33::Rendering::Memory>                 m_pMemory           = nullptr;
    ::std::shared_ptr<::B33::Rendering::VoxelPipeline>          m_pPipeline         = nullptr;

    ::VkCommandPool m_CommandPool = VK_NULL_HANDLE;

    ::size_t                        m_uCurrentFrame;
    ::std::unique_ptr<FramesArray>  m_vFrames = nullptr;

    ::std::shared_ptr<::B33::Rendering::GPUBuffer>          m_VoxelBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>          m_PositionsBuffer;    
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>          m_RotationsBuffer;    
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>          m_HalfSizesBuffer;    
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer>    m_StageVoxelBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer>    m_StagePositonsBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer>    m_StageRotationsBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer>    m_StageHalfSizesBuffer;

    uint32_t m_uStorageBuffersFlags;

    bool m_bDebugMode;
};

} // !B33::Rendering
#endif // !AB_RENDERER_H
