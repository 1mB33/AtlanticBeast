SET(BEE_APPLICATION_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/AppStatus.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/AppResources.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/BaseWindowDetails.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Linux/X11ErrorHandling.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Linux/BasicLinuxPolicy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Linux/GameLinuxPolicy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Win32/BasicWin32Policy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Win32/WindowModeGameWin32Policy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/UserInput.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/IBindMap.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/KeysMap.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/KeysMap.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/MouseMap.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/MouseMap.hpp"
)
SET(BEE_APPLICATION_HEADRES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33App.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AppStatus.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AppResources.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/IWindowPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Linux/BasicLinuxPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Linux/GameLinuxPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Win32/BasicWin32Policy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Win32/WindowModeGameWin32Policy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Win32/BorderlessGameWin32Policy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/BasicSystemPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/GameSystemPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/BaseWindowDetails.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/IBaseWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowListener.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowDesc.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowEvents.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/UserInput.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/Bind.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/InputEvents.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/KeyList.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/MouseButtonList.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/ControllerObject.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/EmptyWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/EmptyCanvas.hpp"
)

ADD_LIBRARY(B33App SHARED
    "${BEE_APPLICATION_SOURCE}"
    "${BEE_APPLICATION_HEADRES}"
)

IF (MSVC)

    TARGET_COMPILE_OPTIONS(B33App PRIVATE "/permissive-")
    TARGET_COMPILE_OPTIONS(B33App PRIVATE "/GR-")

ELSE()

    IF (NOT CMAKE_BUILD_TYPE=="Debug")

        TARGET_COMPILE_OPTIONS(B33App PRIVATE "-O2")

    ENDIF()

ENDIF()

IF (UNIX)

    FIND_PACKAGE(X11 REQUIRED)
    FIND_LIBRARY(XI_LIB Xi REQUIRED)

    TARGET_INCLUDE_DIRECTORIES(B33App PUBLIC ${X11_INCLUDE_DIR})
    TARGET_INCLUDE_DIRECTORIES(B33App PUBLIC ${XI_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(B33App PUBLIC ${X11_LIBRARIES})
    TARGET_LINK_LIBRARIES(B33App PUBLIC ${XI_LIB})

endif()

TARGET_INCLUDE_DIRECTORIES(B33App PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_LINK_LIBRARIES(B33App PUBLIC 
    B33Core
)

TARGET_PRECOMPILE_HEADERS(B33App PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33App.h")

TARGET_COMPILE_DEFINITIONS(B33App PRIVATE _BEAST_EXPORTS)


