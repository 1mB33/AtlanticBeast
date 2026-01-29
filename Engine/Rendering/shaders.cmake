function(compileShaders SHADERS_PATH COMPILE_TARGET)

    SET(SHADER_SRC_DIR "${SHADERS_PATH}")
    SET(SHADER_BIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Shaders")

    FILE(GLOB SHADERS "${SHADER_SRC_DIR}/*.comp")
    FILE(MAKE_DIRECTORY "${SHADER_BIN_DIR}")

    SET(COMPILED_SHADERS)

    IF (SHADERS)

        FOREACH(SHADER_PATH ${SHADERS})
        
            GET_FILENAME_COMPONENT(SHADER_NAME ${SHADER_PATH} NAME)
            SET(SPIRV_PATH "${SHADER_BIN_DIR}/${SHADER_NAME}.spv")
            MESSAGE("COMMAND glslangValidator -V -I${SHADER_SRC_DIR}/ ${SHADER_PATH} -o ${SPIRV_PATH}")
        
            ADD_CUSTOM_COMMAND(
                OUTPUT ${SPIRV_PATH}
                COMMAND glslangValidator -V -I${SHADER_SRC_DIR}/ ${SHADER_PATH} -o ${SPIRV_PATH}
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
