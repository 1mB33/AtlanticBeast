#include "Vulkan/RTXDeviceAdapter.hpp"

#include "B33Rendering.hpp"

namespace B33::Rendering
{

using namespace std;

// --------------------------------------------------------------------------------------------------------------------
const vector<const char *> RTXDeviceAdapter::GetExtensionsImpl() const
{
    static const vector<const char *> vpszDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
    };

    return vpszDeviceExtensions;
}

// --------------------------------------------------------------------------------------------------------------------
void *RTXDeviceAdapter::GetFeaturesImpl() const
{
    static VkPhysicalDeviceRayTracingPipelineFeaturesKHR rayTracingPipelineFeatures = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR,
        NULL,
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

    return &accelerationStructureFeatures;
}

} // namespace B33::Rendering
