#ifndef AB_SWAPCHAIN_H
#define AB_SWAPCHAIN_H

#include "B33Rendering.hpp"
#include "Instance.hpp"
#include "WrapperHardware.hpp"
#include "WrapperAdapter.hpp"
#include "Window/WindowDesc.hpp"

namespace B33::Rendering
{

class Swapchain
{

	friend class Renderer;

public:
    
    static constexpr VkFormat TargetedFormat = VK_FORMAT_B8G8R8A8_UNORM;

public:

    Swapchain() = default;

    BEAST_API Swapchain(::std::shared_ptr<const Instance> inst,
                        ::std::shared_ptr<const HardwareWrapper> hw,
                        ::std::shared_ptr<const AdapterWrapper> da,
                        ::std::shared_ptr<const WindowDesc> wd);

    BEAST_API ~Swapchain();

public:

    Swapchain(Swapchain&&) noexcept = default;
    Swapchain& operator=(Swapchain&&) noexcept = delete;

    Swapchain(const Swapchain&) noexcept = delete;
    Swapchain& operator=(const Swapchain&) noexcept = delete;

public:

    VkSwapchainKHR& GetSwapChainHandle() 
    { return m_pSwapChain; }

    VkImage GetImage(uint32_t i) const
    { 
        AB_ASSERT(i < m_SwapChainImages.size());
        return m_SwapChainImages[i]; 
    }

private:
    
    VkSurfaceKHR CreateSurface(::std::shared_ptr<const Instance>& pInstance,
                               ::std::shared_ptr<const WindowDesc>& pWindowDesc);

    VkSurfaceCapabilitiesKHR GetCapabilitesInternal(::std::shared_ptr<const HardwareWrapper> pHardware, 
                                                    VkSurfaceKHR surface);

    VkExtent2D GetExtentInternal(const VkSurfaceCapabilitiesKHR& capabilities, 
                                 ::std::shared_ptr<const WindowDesc> pWindowDesc);
    
    uint32_t GetImageCountInternal(const VkSurfaceCapabilitiesKHR& capabilities);

    VkSurfaceFormatKHR PickFormat(::std::shared_ptr<const HardwareWrapper>& pHardware,
                                  VkSurfaceKHR surface);

    VkPresentModeKHR PickPresentationMode(::std::shared_ptr<const HardwareWrapper>& pHardware, 
                                          VkSurfaceKHR surface);

    VkSwapchainKHR CreateSwapChain(::std::shared_ptr<const AdapterWrapper>& pAdapter,
                                   VkSurfaceKHR surface,
                                   const VkSurfaceCapabilitiesKHR& capabilities,
                                   const VkExtent2D& extent2D,
                                   uint32_t uImageCount,
                                   const VkSurfaceFormatKHR& surfaceFormat,
                                   VkPresentModeKHR presentMode);

    uint32_t GetNumberOfSwapChainImages(::std::shared_ptr<const AdapterWrapper>& pAdapter, 
                                        VkSwapchainKHR swapchain);

    ::std::vector<VkImage> CreateSwapChainImages(::std::shared_ptr<const AdapterWrapper>& pAdapter,
                                                 VkSwapchainKHR swapchain,
                                                 uint32_t uAmount);

private:

    ::std::shared_ptr<const Instance>           m_pInstance         = nullptr;
    ::std::shared_ptr<const HardwareWrapper>    m_pHardware         = nullptr;
    ::std::shared_ptr<const AdapterWrapper>     m_pDeviceAdapter    = nullptr;
    ::std::shared_ptr<const WindowDesc>         m_pWindowDesc       = nullptr;

    VkSurfaceKHR                m_Surface       = VK_NULL_HANDLE;
    VkSurfaceCapabilitiesKHR    m_Capabilities  = { };
    VkExtent2D                  m_Extent        = { 0, 0 };
    const uint32_t              m_uImageCount   = -1;
    VkSurfaceFormatKHR          m_SurfaceFormat = { };
    VkPresentModeKHR            m_PresentMode   = { };
    VkSwapchainKHR              m_pSwapChain    = VK_NULL_HANDLE;

    uint32_t                m_uCurrentImageIndex = 0;
    ::std::vector<VkImage>  m_SwapChainImages;

 };

} // !B33::Rendering
#endif // !AB_SWAPCHAIN_H
