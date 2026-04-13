#ifndef B33_VOXELS_H
#define B33_VOXELS_H

#ifdef _WIN32
#    define VK_USE_PLATFORM_WIN32_KHR
#elif __linux
#    define VK_USE_PLATFORM_XLIB_KHR
#endif

#include "B33App.h"
#include "B33Core.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#ifdef __cplusplus
#    include "B33Math.hpp"

#    ifdef _WIN32
#        undef LoadImage
#    endif // !_WIN32
#endif     // !__cplusplus
#endif     // !B33_VOXELS_H
