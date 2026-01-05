SET(BEE_CORE_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Exception.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Debug/Logger.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Core.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Synchronization/DeltaTime.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Synchronization/FpsLimiter.cpp"
)
SET(BEE_CORE_HEADRES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/CStandard.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/CPlusPlusStandard.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/ExportImport.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/MinimalLinuxIncludes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/MinimalWindowsIncludes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/MinimalSystemIncludes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/CoreMinimal.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AbLimits.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Exception.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Debug/Assert.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Debug/Logger.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Synchronization/FpsLimiter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Synchronization/DeltaTime.hpp"
)

ADD_LIBRARY(BeeCore SHARED
    "${BEE_CORE_SOURCE}"
    "${BEE_CORE_HEADRES}"
)

IF (MSVC)

    TARGET_COMPILE_OPTIONS(BeeCore PRIVATE "/permissive-")
    TARGET_COMPILE_OPTIONS(BeeCore PRIVATE "/GR-")

ELSE()

    IF (NOT CMAKE_BUILD_TYPE=="Debug")

        TARGET_COMPILE_OPTIONS(BeeCore PRIVATE "-O2")

    ENDIF()

ENDIF()

TARGET_INCLUDE_DIRECTORIES(BeeCore PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_PRECOMPILE_HEADERS(BeeCore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/Core.h")

TARGET_COMPILE_DEFINITIONS(BeeCore PRIVATE _BEAST_EXPORTS)

