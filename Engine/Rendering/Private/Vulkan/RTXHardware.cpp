#include "B33Rendering.hpp"

#include "Vulkan/RTXHardware.hpp"

namespace B33::Rendering
{

using namespace std;

// RTXHardware // ------------------------------------------------------------------------------------------------------
RTXHardware::RTXHardware(shared_ptr<const Instance> pInstance)
    : HardwareWrapper(pInstance, ChooseGPU(pInstance))
{ 
    AB_LOG(Core::Debug::Info, L"Creating a hardware!");
}

// ---------------------------------------------------------------------------------------------------------------------
VkPhysicalDevice RTXHardware::ChooseGPU(const shared_ptr<const Instance>& pInstance)
{ 
    VkPhysicalDevice            chosenPhysicalDevice    = VK_NULL_HANDLE;
    vector<VkPhysicalDevice>    vPhysicalDevices        = GetPhysicalDevices(pInstance->GetInstance());

    VkPhysicalDeviceProperties deviceProperties;

    VkPhysicalDeviceRayTracingPipelineFeaturesKHR       rayTracingPipelineFeatures;
    VkPhysicalDeviceAccelerationStructureFeaturesKHR    accelStructFeatures;
    VkPhysicalDeviceBufferDeviceAddressFeatures         bufferAddressFeatures;

    VkPhysicalDeviceFeatures2 deviceFeatures2;

    for (const auto& pDevice : vPhysicalDevices) 
    {
        vkGetPhysicalDeviceProperties(pDevice, &deviceProperties);

        if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            continue;
        }

        memset(static_cast<void*>(&rayTracingPipelineFeatures), 0, sizeof(rayTracingPipelineFeatures));
        memset(static_cast<void*>(&accelStructFeatures), 0, sizeof(accelStructFeatures));
        memset(static_cast<void*>(&bufferAddressFeatures), 0, sizeof(bufferAddressFeatures));
        memset(static_cast<void*>(&deviceFeatures2), 0, sizeof(deviceFeatures2));

        rayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
        rayTracingPipelineFeatures.pNext = NULL;

        accelStructFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        accelStructFeatures.pNext = &rayTracingPipelineFeatures;

        bufferAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
        bufferAddressFeatures.pNext = &accelStructFeatures;

        deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        deviceFeatures2.pNext = &bufferAddressFeatures;
    
        vkGetPhysicalDeviceFeatures2(pDevice, &deviceFeatures2);

        if (rayTracingPipelineFeatures.rayTracingPipeline &&
            accelStructFeatures.accelerationStructure &&
            bufferAddressFeatures.bufferDeviceAddress) 
        {
            chosenPhysicalDevice = pDevice;
            break;
        }
    }

    if (chosenPhysicalDevice == VK_NULL_HANDLE) {
        AB_LOG(Core::Debug::Error, L"Ohh nooo... Couldn't choose a valid physical gpu!!!");
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }

    return chosenPhysicalDevice;
}

} // !B33::Rendering
