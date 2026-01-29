#include "B33Rendering.hpp"

#include "Vulkan/ComputeAdapter.hpp"

namespace B33::Rendering
{

using namespace std;

// --------------------------------------------------------------------------------------------------------------------
const vector<const char*>& ComputeAdapter::GetExtensionsImpl() const
{
    static const vector<const char*> vpszDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    return vpszDeviceExtensions;
}

// --------------------------------------------------------------------------------------------------------------------
void* ComputeAdapter::GetFeaturesImpl() const
{
    static VkPhysicalDeviceTimelineSemaphoreFeatures semaphoreFeatures = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES,
        NULL,
        VK_TRUE
    };
    static VkPhysicalDevice8BitStorageFeatures bitStorageFeatures = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR,
        &semaphoreFeatures,
        VK_TRUE,
        VK_FALSE,
        VK_FALSE
    };
    static VkPhysicalDeviceVulkanMemoryModelFeatures memoryModelFeatures = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES,
        &bitStorageFeatures,
        VK_TRUE,
        VK_TRUE,
        VK_FALSE,
    };

    return &memoryModelFeatures;
}

};
