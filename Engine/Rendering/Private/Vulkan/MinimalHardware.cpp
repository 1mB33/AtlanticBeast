#include "B33Rendering.hpp"

#include "Vulkan/MinimalHardware.hpp"
#include "Vulkan/ErrorHandling.hpp"

namespace B33::Rendering
{

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
MinimalHardware::MinimalHardware()
  : IHardware()
{
}

// ---------------------------------------------------------------------------------------------------------------------
VkPhysicalDevice MinimalHardware::ChooseHardwareImpl( const shared_ptr<const Instance> &pInstance ) const
{
    uint32_t                   uDeviceCount;
    vector<VkPhysicalDevice>   vPhysicalDevices;
    VkPhysicalDevice           chosenPhysicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties deviceProperties;

    THROW_IF_FAILED( vkEnumeratePhysicalDevices( pInstance->GetInstance(), &uDeviceCount, NULL ) );

    vPhysicalDevices.resize( uDeviceCount );
    if ( !uDeviceCount )
    {
        B33_LOG( Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!! No GPU found!" );
        throw B33_EXCEPT( "Ohh nooo... Vulkan isn't working!!!" );
    }

    THROW_IF_FAILED( vkEnumeratePhysicalDevices( pInstance->GetInstance(), &uDeviceCount, &vPhysicalDevices[ 0 ] ) );

    for ( const auto &pDevice : vPhysicalDevices )
    {
        vkGetPhysicalDeviceProperties( pDevice, &deviceProperties );

        if ( chosenPhysicalDevice == VK_NULL_HANDLE && pDevice != VK_NULL_HANDLE )
        {
            chosenPhysicalDevice = pDevice;
        }

        if ( deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
        {
            continue;
        }

        chosenPhysicalDevice = pDevice;
        break;
    }

    if ( chosenPhysicalDevice == VK_NULL_HANDLE )
    {
        B33_LOG( Core::Debug::Error, L"Ohh nooo... Couldn't choose a valid physical gpu!!!" );
        throw B33_EXCEPT( "Ohh nooo... Vulkan isn't working!!!" );
    }

    return chosenPhysicalDevice;
}

} // namespace B33::Rendering
