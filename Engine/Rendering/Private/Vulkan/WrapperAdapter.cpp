#include "B33Core.h"
#include "B33Rendering.hpp"

#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

using namespace std;

// --------------------------------------------------------------------------------------------------------------------
uint32_t AdapterWrapper::ChooseQueueFamily( VkPhysicalDevice gpu, const uint32_t uFlags ) const
{
    B33_TRACE( L"AdapterWrapper::ChooseQueueFamily()" );
    uint32_t                        uFamilyCount;
    vector<VkQueueFamilyProperties> vProperties    = {};
    VkPhysicalDevice                physicalDevice = gpu;

    vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &uFamilyCount, NULL );
    if ( !uFamilyCount )
    {
        B33_LOG( Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!!" );
        throw B33_EXCEPT( "Ohh nooo... Vulkan isn't working!!!" );
    }
    vProperties.resize( uFamilyCount );

    vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &uFamilyCount, &vProperties[ 0 ] );
    if ( !uFamilyCount )
    {
        B33_LOG( Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!!" );
        throw B33_EXCEPT( "Ohh nooo... Vulkan isn't working!!!" );
    }

    for ( uint32_t i = 0; i < uFamilyCount; ++i )
        if ( vProperties[ i ].queueFlags & uFlags )
            return i;

    B33_LOG( Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!!" );
    throw B33_EXCEPT( "Ohh nooo... Vulkan isn't working!!!" );
}

// --------------------------------------------------------------------------------------------------------------------
VkDevice AdapterWrapper::CreateDevice( VkPhysicalDevice            gpu,
                                       const vector<const char *> &vExtensions,
                                       const void                 *pFeatures,
                                       const uint32_t              uFamilyIndex ) const
{
    B33_TRACE( L"AdapterWrapper::CreateDevice()" );
    VkDevice device            = VK_NULL_HANDLE;
    float    queuePriorities[] = { 1. };

    VkDeviceQueueCreateInfo queueCreateInfo = {
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = uFamilyIndex,
        .queueCount       = size( queuePriorities ),
        .pQueuePriorities = queuePriorities,
    };

    VkDeviceCreateInfo createInfo = {
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = pFeatures,
        .queueCreateInfoCount    = 1,
        .pQueueCreateInfos       = &queueCreateInfo,
        .enabledExtensionCount   = static_cast<uint32_t>( !vExtensions.empty() ? vExtensions.size() : 0 ),
        .ppEnabledExtensionNames = !vExtensions.empty() ? &vExtensions[ 0 ] : NULL,
        .pEnabledFeatures        = NULL,
    };

    THROW_IF_FAILED( vkCreateDevice( gpu, &createInfo, NULL, &device ) );

    return device;
}

// --------------------------------------------------------------------------------------------------------------------
VkQueue AdapterWrapper::CreateQueue( VkDevice dv, const uint32_t uQueueIndex ) const
{
    B33_TRACE( L"AdapterWrapper::CreateQueue()" );
    VkQueue graphicsQueue;

    vkGetDeviceQueue( dv, uQueueIndex, 0, &graphicsQueue );

    return graphicsQueue;
}

} // namespace B33::Rendering
