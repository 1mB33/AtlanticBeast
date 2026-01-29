SET(BEE_CORE_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/B33Core.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Exception.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Debug/Logger.cpp"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AbLimits.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Exception.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Debug/Assert.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Debug/Logger.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33CoreMinimal.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Synchronization/FpsLimiter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Synchronization/DeltaTime.hpp"
)

ADD_LIBRARY(B33Core SHARED
    "${BEE_CORE_SOURCE}"
    "${BEE_CORE_HEADRES}"
)

IF (MSVC)

    TARGET_COMPILE_OPTIONS(B33Core PRIVATE "/permissive-")
    TARGET_COMPILE_OPTIONS(B33Core PRIVATE "/GR-")

ELSE()

    IF (NOT CMAKE_BUILD_TYPE=="Debug")

        TARGET_COMPILE_OPTIONS(B33Core PRIVATE "-O2")

    ENDIF()

ENDIF()

TARGET_INCLUDE_DIRECTORIES(B33Core PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_PRECOMPILE_HEADERS(B33Core PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33Core.h")

TARGET_COMPILE_DEFINITIONS(B33Core PRIVATE _BEAST_EXPORTS)

