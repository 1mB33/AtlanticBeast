#include "B33Rendering.hpp"

#include "Vulkan/SwapChain.hpp"

#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/WrapperHardware.hpp"


namespace B33::Rendering
{

using namespace std;

// Swapchain // --------------------------------------------------------------------------------------------------------
Swapchain::Swapchain(shared_ptr<const Instance> pInst, 
                     shared_ptr<const HardwareWrapper> hw,
                     shared_ptr<const AdapterWrapper> da,
                     shared_ptr<const WindowDesc> wd)
    : m_pInstance(pInst)
    , m_pHardware(hw)
    , m_pDeviceAdapter(da)
    , m_pWindowDesc(wd)
    , m_Surface(CreateSurface(m_pInstance, m_pWindowDesc))
    , m_Capabilities(GetCapabilitesInternal(m_pHardware, m_Surface))
    , m_Extent(GetExtentInternal(m_Capabilities, m_pWindowDesc))
    , m_uImageCount(GetImageCountInternal(m_Capabilities))
    , m_SurfaceFormat(PickFormat(m_pHardware, m_Surface))
    , m_PresentMode(PickPresentationMode(m_pHardware, m_Surface))
    , m_pSwapChain(CreateSwapChain(m_pDeviceAdapter,
                                   m_Surface,
                                   m_Capabilities,
                                   m_Extent,
                                   m_uImageCount,
                                   m_SurfaceFormat,
                                   m_PresentMode))
    , m_uCurrentImageIndex(0)
    , m_SwapChainImages(CreateSwapChainImages(m_pDeviceAdapter, m_pSwapChain, m_uImageCount))
{
    AB_LOG(Core::Debug::Info, L"Creating a swapchain!");
}

// ---------------------------------------------------------------------------------------------------------------------
Swapchain::~Swapchain()
{
    if (m_pSwapChain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(m_pDeviceAdapter->GetAdapterHandle(), m_pSwapChain, NULL);
        m_pSwapChain = VK_NULL_HANDLE;
    }
    if (m_Surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(m_pInstance->GetInstance(), m_Surface, NULL);
        m_Surface = VK_NULL_HANDLE;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
VkSurfaceKHR Swapchain::CreateSurface(shared_ptr<const Instance>& pInstance, shared_ptr<const WindowDesc>& pWindowDesc)
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;

#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR createInfo;
    createInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext     = NULL;
    createInfo.flags     = 0;
    createInfo.hinstance = GetModuleHandle(NULL);
    createInfo.hwnd      = pWindowDesc->hWnd;

    THROW_IF_FAILED(vkCreateWin32SurfaceKHR(pInstance->GetInstance(),
                                            &createInfo,
                                            NULL,
                                            &surface));
#elif __linux__
    VkXlibSurfaceCreateInfoKHR  createInfo;
    createInfo.sType    = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext    = NULL;
    createInfo.flags    = 0;
    createInfo.dpy      = pWindowDesc->pDisplayHandle;
    createInfo.window   = pWindowDesc->WindowHandle;

    THROW_IF_FAILED(vkCreateXlibSurfaceKHR(pInstance->GetInstance(),
                                           &createInfo,
                                           NULL,
                                           &surface));
#endif // !_WIN32
       
    return surface;
}

// ---------------------------------------------------------------------------------------------------------------------
VkSurfaceCapabilitiesKHR Swapchain::GetCapabilitesInternal(shared_ptr<const HardwareWrapper> pHardware,
                                                           VkSurfaceKHR surface)
{
    VkSurfaceCapabilitiesKHR capabilities;

    THROW_IF_FAILED(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pHardware->GetPhysicalDevice(), 
                                                              surface,
                                                              &capabilities));

    if ((capabilities.supportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT) == 0) {
        AB_LOG(Core::Debug::Error, L"VK_IMAGE_USAGE_STORAGE_BIT not supported for swapchain images.");
        throw AB_EXCEPT("Ohh no...  vulkan isn't working");
    }

    return capabilities;
}

// ---------------------------------------------------------------------------------------------------------------------
uint32_t Swapchain::GetImageCountInternal(const VkSurfaceCapabilitiesKHR& capabilities)
{
    uint32_t uImageCount;

    uImageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && uImageCount > capabilities.maxImageCount) {
        uImageCount = capabilities.maxImageCount;
    }

    return uImageCount;
}

// ---------------------------------------------------------------------------------------------------------------------
VkExtent2D Swapchain::GetExtentInternal(const VkSurfaceCapabilitiesKHR& capabilities,
                                        shared_ptr<const WindowDesc> pWindowDesc )
{
    VkExtent2D extent = capabilities.currentExtent;

    if (extent.width == UINT32_MAX) {
        extent.width    = pWindowDesc->Width;
        extent.height   = pWindowDesc->Height;
    }

    return extent;
}

// ---------------------------------------------------------------------------------------------------------------------
VkSwapchainKHR Swapchain::CreateSwapChain(shared_ptr<const AdapterWrapper>& pAdapter,
                                          VkSurfaceKHR surface,
                                          const VkSurfaceCapabilitiesKHR& capabilities,
                                          const VkExtent2D& extent2D,
                                          uint32_t uImageCount,
                                          const VkSurfaceFormatKHR& surfaceFormat,
                                          VkPresentModeKHR presentMode)
{
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;

    VkSwapchainCreateInfoKHR swapchainInfo = { };
    swapchainInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface               = surface;
    swapchainInfo.minImageCount         = uImageCount;
    swapchainInfo.imageFormat           = surfaceFormat.format;
    swapchainInfo.imageColorSpace       = surfaceFormat.colorSpace;
    swapchainInfo.imageExtent           = extent2D;
    swapchainInfo.imageArrayLayers      = 1;
    swapchainInfo.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
    swapchainInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    swapchainInfo.preTransform          = capabilities.currentTransform;
    swapchainInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.presentMode           = presentMode;
    swapchainInfo.clipped               = VK_TRUE;

    THROW_IF_FAILED(vkCreateSwapchainKHR(pAdapter->GetAdapterHandle(),
                                         &swapchainInfo,
                                         NULL,
                                         &swapChain));

    return swapChain;
}

// ---------------------------------------------------------------------------------------------------------------------
VkSurfaceFormatKHR Swapchain::PickFormat(shared_ptr<const HardwareWrapper>& pHardware, VkSurfaceKHR surface)
{
    VkPhysicalDevice            physicalDeviceHandle    = pHardware->GetPhysicalDevice();
    uint32_t                    uFormatCount            = 0;
    vector<VkSurfaceFormatKHR>  vFormats                = { };
    bool                        bPicked                 = false;
    size_t                      choosenFormatIndex;
    
    THROW_IF_FAILED(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceHandle,
                                                       surface,
                                                       &uFormatCount,
                                                       NULL));
    

    vFormats.resize(uFormatCount);
    THROW_IF_FAILED(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceHandle,
                                                         surface,
                                                         &uFormatCount,
                                                         &vFormats[0]));

    AB_ASSERT(!vFormats.empty());

    for (size_t i = 0; i < vFormats.size(); ++i)
    {
        const auto& format = vFormats[i];

        if (format.format == Swapchain::TargetedFormat && 
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
        {
            return vFormats[i];
        }

        if (format.format == Swapchain::TargetedFormat && !bPicked) {
            choosenFormatIndex = i;
            bPicked = true;
        }
    }

    if (!bPicked) {
        AB_LOG(Core::Debug::Warning, L"Chosen random surface format");
        return vFormats[0];
    } else {
        AB_LOG(Core::Debug::Warning, L"Picked random color space");
    }

    return vFormats[choosenFormatIndex];
}

// ---------------------------------------------------------------------------------------------------------------------
VkPresentModeKHR Swapchain::PickPresentationMode(shared_ptr<const HardwareWrapper>& pHardware, VkSurfaceKHR surface)
{ 
    VkPhysicalDevice            physicalDeviceHandle    = pHardware->GetPhysicalDevice();
    uint32_t                    uPresentModeCount       = 0;
    vector<VkPresentModeKHR>    vPresentModes           = { };

    THROW_IF_FAILED(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDeviceHandle,
                                                              surface,
                                                              &uPresentModeCount,
                                                              NULL));
    vPresentModes.resize(uPresentModeCount);
    THROW_IF_FAILED(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDeviceHandle,
                                                              surface,
                                                              &uPresentModeCount,
                                                              &vPresentModes[0]));
    AB_ASSERT(!vPresentModes.empty());

    for (const auto& mode : vPresentModes) 
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) 
            return mode;


    return VK_PRESENT_MODE_FIFO_KHR;
}
 
// ---------------------------------------------------------------------------------------------------------------------
uint32_t Swapchain::GetNumberOfSwapChainImages(shared_ptr<const AdapterWrapper>& pAdapter, 
                                               VkSwapchainKHR swapchain)
{
    uint32_t uImageCount = 0;

    THROW_IF_FAILED(vkGetSwapchainImagesKHR(pAdapter->GetAdapterHandle(), swapchain, &uImageCount, nullptr));

    return uImageCount;
}

// ---------------------------------------------------------------------------------------------------------------------
vector<VkImage> Swapchain::CreateSwapChainImages(shared_ptr<const AdapterWrapper>& pAdapter,
                                                 VkSwapchainKHR swapchain,
                                                 uint32_t uAmount)
{
    vector<VkImage> swapChainImages(uAmount);

    THROW_IF_FAILED(vkGetSwapchainImagesKHR(pAdapter->GetAdapterHandle(),
                                            swapchain,
                                            &uAmount,
                                            &swapChainImages[0]));
    
    return swapChainImages;
}

} // !B33::Rendering
