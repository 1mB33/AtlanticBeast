#ifndef B33_VOXELS_H
#define B33_VOXELS_H

#if defined( _WIN32 )
#    define VK_USE_PLATFORM_WIN32_KHR
#elif defined( _X11 )
#    define VK_USE_PLATFORM_XLIB_KHR
#endif

#include "B33App.h"
#include "B33Core.h"

#if defined( _WIN32 ) || defined( __linux__ ) || defined( __APPLE__ )
#    include <vulkan/vulkan.h>
#    include <vulkan/vulkan_core.h>
#endif

#ifdef __cplusplus
#    include "B33Math.hpp"

#    ifdef _WIN32
#        undef LoadImage
#    endif // !_WIN32
#endif     // !__cplusplus
#endif     // !B33_VOXELS_H
