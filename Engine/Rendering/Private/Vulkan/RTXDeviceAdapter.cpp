#include "B33Rendering.hpp"

#include "Vulkan/RTXDeviceAdapter.hpp"

namespace B33::Rendering
{

using namespace std;

// --------------------------------------------------------------------------------------------------------------------
const ::std::vector<const char*> RTXDeviceAdapter::GetExtensionsImpl() const
{
    static const std::vector<const char*> vpszDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    };

    return vpszDeviceExtensions;
}

// --------------------------------------------------------------------------------------------------------------------
void* RTXDeviceAdapter::GetFeaturesImpl() const
{
    static VkPhysicalDeviceTimelineSemaphoreFeatures semaphoreFeatures = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES,
        NULL,
        VK_TRUE,
    };
    static VkPhysicalDevice8BitStorageFeatures bitStorageFeatures = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR,
        &semaphoreFeatures,
        VK_TRUE,
        VK_FALSE,
        VK_FALSE,
    };
    static VkPhysicalDeviceVulkanMemoryModelFeatures memoryModelFeatures = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES,
        &bitStorageFeatures,
        VK_TRUE,
        VK_TRUE,
        VK_FALSE,
    };
    static VkPhysicalDeviceRayTracingPipelineFeaturesKHR rayTracingPipelineFeatures = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR,
        &memoryModelFeatures,
        VK_TRUE,
        VK_FALSE,
        VK_FALSE,
        VK_FALSE,
        VK_TRUE,
    };
    static VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR,
        &rayTracingPipelineFeatures,
        VK_TRUE,
        VK_FALSE,
        VK_FALSE,
        VK_FALSE,
        VK_FALSE,
    };
    static VkPhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES,
        &accelerationStructureFeatures,
        VK_TRUE,
        VK_FALSE,
        VK_FALSE,                              
    };

    return &bufferDeviceAddressFeatures;
}

} // !B33::Rendering

