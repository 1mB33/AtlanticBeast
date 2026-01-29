#ifndef AB_VOXELS_H
#define AB_VOXELS_H

#ifdef _WIN32
#   define VK_USE_PLATFORM_WIN32_KHR
#elif __linux
#   define VK_USE_PLATFORM_XLIB_KHR
#endif

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "Core.h"
#include "Math.hpp"
#include "App.h"

#ifdef __cplusplus
#   include <vulkan/vulkan.hpp>
#   ifdef _WIN32
#      undef LoadImage
#   endif // !_WIN32
#endif // !__cplusplus
#endif // !AB_VOXELS_H
