function(compileShadersGlslang SHADERS_PATH COMPILE_TARGET)

    SET(SHADER_SRC_DIR "${SHADERS_PATH}")
    SET(SHADER_BIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Shaders")

    FILE(GLOB SHADERS "${SHADER_SRC_DIR}/glsl/*.comp.*")
    FILE(MAKE_DIRECTORY "${SHADER_BIN_DIR}")

    SET(COMPILED_SHADERS)

    IF (SHADERS)

        FOREACH(SHADER_PATH ${SHADERS})
        
            GET_FILENAME_COMPONENT(SHADER_NAME ${SHADER_PATH} NAME_WE)
            SET(SPIRV_PATH "${SHADER_BIN_DIR}/${SHADER_NAME}.spv")
            MESSAGE("COMMAND glslangValidator -e main -V -I${SHADER_SRC_DIR}/ ${SHADER_PATH} -o ${SPIRV_PATH}")
        
            ADD_CUSTOM_COMMAND(
                OUTPUT ${SPIRV_PATH}
                COMMAND glslangValidator -e main -V -I${SHADER_SRC_DIR}/ ${SHADER_PATH} -o ${SPIRV_PATH}
                DEPENDS ${SHADER_PATH}
                COMMENT "Compiling shader ${SHADER_NAME}"
                VERBATIM
            )
        
            LIST(APPEND COMPILED_SHADERS ${SPIRV_PATH})
        
        ENDFOREACH()

        ADD_CUSTOM_TARGET(${COMPILE_TARGET} ALL
            DEPENDS ${COMPILED_SHADERS}
            COMMENT "Compiling shaders"
        )

    ELSE()

        MESSAGE(WARNING "No .comp shaders found in: ${SHADER_SRC_DIR}")

    ENDIF()

endfunction()

function(compileShadersDXC SHADERS_PATH COMPILE_TARGET)

    SET(SHADER_SRC_DIR "${SHADERS_PATH}")
    SET(SHADER_BIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Shaders")

    FILE(GLOB SHADERS "${SHADER_SRC_DIR}/hlsl/*.comp.*")
    FILE(MAKE_DIRECTORY "${SHADER_BIN_DIR}")

    SET(COMPILED_SHADERS)

    IF (SHADERS)

        FOREACH(SHADER_PATH ${SHADERS})
        
            GET_FILENAME_COMPONENT(SHADER_NAME ${SHADER_PATH} NAME_WE)
            SET(SPIRV_PATH "${SHADER_BIN_DIR}/${SHADER_NAME}.spv")
            MESSAGE("COMMAND dxc -T cs_6_0 -E main -spirv ${SHADER_PATH} -Fo ${SPIRV_PATH}")
        
            ADD_CUSTOM_COMMAND(
                OUTPUT ${SPIRV_PATH}
                COMMAND dxc -T cs_6_0 -E main -spirv -D VULKAN ${SHADER_PATH} -Fo ${SPIRV_PATH}
                DEPENDS ${SHADER_PATH}
                COMMENT "Compiling shader ${SHADER_NAME}"
                VERBATIM
            )
        
            LIST(APPEND COMPILED_SHADERS ${SPIRV_PATH})
        
        ENDFOREACH()

        ADD_CUSTOM_TARGET(${COMPILE_TARGET} ALL
            DEPENDS ${COMPILED_SHADERS}
            COMMENT "Compiling shaders"
        )

    ELSE()

        MESSAGE(WARNING "No .comp shaders found in: ${SHADER_SRC_DIR}")

    ENDIF()

endfunction()


