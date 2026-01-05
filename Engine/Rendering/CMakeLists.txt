SET(BEE_RENDERING_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Instance.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/WrapperHardware.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/WrapperAdapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/MinimalHardware.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/RTXHardware.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/ComputeAdapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/RTXDeviceAdapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/SwapChain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Memory.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/Rays.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/VoxelPipeline.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/VoxelGrid.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/Renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Voxels.cpp"
)
SET(BEE_RENDERING_HEADRES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Primitives/Camera.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/ErrorHandling.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/GPUBuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Instance.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/MinimalHardware.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/RTXHardware.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/ComputeAdapter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/RTXDeviceAdapter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/SwapChain.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Memory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/FrameResources.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/VoxelPipeline.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/PushConstants.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/SingleVoxel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/VoxelGrid.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/Renderer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/Rays.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Voxels.hpp"
)

ADD_LIBRARY(BeeRendering SHARED 
    "${BEE_RENDERING_SOURCE}"
    "${BEE_RENDERING_HEADRES}"
)

IF (MSVC)

    TARGET_COMPILE_OPTIONS(BeeRendering PUBLIC "/permissive-")
    TARGET_COMPILE_OPTIONS(BeeRendering PUBLIC "/GR-")

ELSE()

    IF (NOT CMAKE_BUILD_TYPE=="Debug")

        TARGET_COMPILE_OPTIONS(BeeRendering PUBLIC "-O2")

    ENDIF()

ENDIF()

FIND_PACKAGE(Vulkan REQUIRED)

TARGET_INCLUDE_DIRECTORIES(BeeRendering PUBLIC 
    "${Vulkan_INCLUDE_DIRS}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_LINK_LIBRARIES(BeeRendering PUBLIC 
    "${Vulkan_LIBRARIES}"
    BeeCore
    BeeMath
    BeeApp
)

TARGET_PRECOMPILE_HEADERS(BeeRendering PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/Voxels.hpp")

TARGET_COMPILE_DEFINITIONS(BeeRendering PRIVATE _BEAST_EXPORTS)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/shaders.cmake)
compileShaders("${CMAKE_CURRENT_SOURCE_DIR}/Public/Shaders" CompileShadersForVoxels)
ADD_DEPENDENCIES(BeeRendering CompileShadersForVoxels)
