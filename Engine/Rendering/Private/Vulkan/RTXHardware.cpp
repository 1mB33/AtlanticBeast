#include "B33Rendering.hpp"

#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/RTXHardware.hpp"

namespace B33::Rendering
{

using namespace std;

// RTXHardware // ------------------------------------------------------------------------------------------------------
RTXHardware::RTXHardware()
  : IHardware()
{
}

// ---------------------------------------------------------------------------------------------------------------------
VkPhysicalDevice RTXHardware::ChooseHardwareImpl( const shared_ptr<const Instance> &pInstance )
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

    VkPhysicalDeviceRayTracingPipelineFeaturesKHR    rayTracingPipelineFeatures;
    VkPhysicalDeviceAccelerationStructureFeaturesKHR accelStructFeatures;

    VkPhysicalDeviceFeatures2 deviceFeatures2;

    for ( const auto &pDevice : vPhysicalDevices )
    {
        vkGetPhysicalDeviceProperties( pDevice, &deviceProperties );

        if ( deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
        {
            continue;
        }

        memset( static_cast<void *>( &rayTracingPipelineFeatures ), 0, sizeof( rayTracingPipelineFeatures ) );
        memset( static_cast<void *>( &accelStructFeatures ), 0, sizeof( accelStructFeatures ) );
        memset( static_cast<void *>( &deviceFeatures2 ), 0, sizeof( deviceFeatures2 ) );

        rayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
        rayTracingPipelineFeatures.pNext = NULL;

        accelStructFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        accelStructFeatures.pNext = &rayTracingPipelineFeatures;

        deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        deviceFeatures2.pNext = &accelStructFeatures;

        vkGetPhysicalDeviceFeatures2( pDevice, &deviceFeatures2 );

        if ( rayTracingPipelineFeatures.rayTracingPipeline && accelStructFeatures.accelerationStructure )
        {
            chosenPhysicalDevice = pDevice;
            break;
        }
    }

    if ( chosenPhysicalDevice == VK_NULL_HANDLE )
    {
        B33_LOG( Core::Debug::Error, L"Ohh nooo... Couldn't choose a valid physical gpu!!!" );
        throw B33_EXCEPT( "Ohh nooo... Vulkan isn't working!!!" );
    }

    return chosenPhysicalDevice;
}

} // namespace B33::Rendering
