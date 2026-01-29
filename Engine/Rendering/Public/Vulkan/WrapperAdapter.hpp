#ifndef AB_WRAPPER_ADAPTER_H
#define AB_WRAPPER_ADAPTER_H

#include "B33Rendering.hpp"
#include "Vulkan/WrapperHardware.hpp"

namespace B33::Rendering
{

class AdapterWrapper
{
public:

    AdapterWrapper() = delete;

    AdapterWrapper(::std::shared_ptr<const HardwareWrapper> pHardware,
                   const uint32_t uFlags,
                   const std::vector<const char*>& vExtensions,
                   const void* pFeatures)
        : m_pGPU(pHardware)
        , m_uQueueFamily(ChooseQueueFamily(m_pGPU->GetPhysicalDevice(), uFlags))
        , m_Device(CreateDevice(m_pGPU->GetPhysicalDevice(), vExtensions, pFeatures, m_uQueueFamily))
        , m_Queue(CreateQueue(m_Device, m_uQueueFamily))
    { 
        AB_LOG(Core::Debug::Info, L"Initializing adapter");
    }
                   
    ~AdapterWrapper()
    {
        AB_LOG(Core::Debug::Info, L"Destroying adapter");
        if (m_Device != VK_NULL_HANDLE) {
            vkDestroyDevice(m_Device, NULL);
            m_Device = VK_NULL_HANDLE;
        }
    }

public:

    AdapterWrapper(AdapterWrapper&&) noexcept = default;
    AdapterWrapper(const AdapterWrapper&) = delete;

    AdapterWrapper& operator=(AdapterWrapper&&) noexcept = default;
    AdapterWrapper& operator=(const AdapterWrapper&) noexcept = delete;

public:

    uint32_t GetQueueFamilyIndex() const
    { return m_uQueueFamily; }

    VkDevice GetAdapterHandle() const
    { return m_Device; }

    VkQueue GetQueueHandle() const
    { return m_Queue; }

private:

    uint32_t ChooseQueueFamily(VkPhysicalDevice gpu, const uint32_t uFlags) const;

    VkDevice CreateDevice(VkPhysicalDevice gpu, 
                          const std::vector<const char*>& vExtensions,
                          const void* pFeatures,
                          const uint32_t uFamilyIndex) const;
    
    VkQueue CreateQueue(VkDevice dv, uint32_t uQueueIndex) const;

private:

    ::std::shared_ptr<const HardwareWrapper> m_pGPU = nullptr;

    uint32_t m_uQueueFamily = 0;

    VkDevice    m_Device    = VK_NULL_HANDLE;
    VkQueue     m_Queue     = VK_NULL_HANDLE;

};

} // !B33::Rendering
#endif // !AB_WRAPPER_ADAPTER_H

