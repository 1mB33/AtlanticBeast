#include "B33Rendering.hpp"

#include "Vulkan/WrapperHardware.hpp"

#include "Vulkan/ErrorHandling.hpp"

namespace B33::Rendering
{

using namespace std;

// --------------------------------------------------------------------------------------------------------------------
vector<VkPhysicalDevice> HardwareWrapper::GetPhysicalDevices(VkInstance instance)
{
    uint32_t                    uDeviceCount;
    vector<VkPhysicalDevice>    vPhysicalDevices;

    THROW_IF_FAILED(vkEnumeratePhysicalDevices(instance,
                                               &uDeviceCount,
                                               NULL));

    vPhysicalDevices.resize(uDeviceCount);
    if (!uDeviceCount) {
        AB_LOG(Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!! No GPU found!");
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }

    THROW_IF_FAILED(vkEnumeratePhysicalDevices(instance,
                                               &uDeviceCount,
                                               &vPhysicalDevices[0]));
    
    return vPhysicalDevices;
}

} // !B33::Rendering

