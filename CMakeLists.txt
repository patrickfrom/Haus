cmake_minimum_required(VERSION 3.27)
project(Haus)

set(CMAKE_CXX_STANDARD 20)

add_executable(Haus main.cpp
        Application.cpp
        Application.h
        vendors/stb/image.h
        vendors/tiny_obj_loader.h)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(vendors/GLFW)
add_subdirectory(vendors/glm)
add_subdirectory(vendors/stb)

find_package(Vulkan REQUIRED)
target_link_libraries(Haus PRIVATE Vulkan::Vulkan glfw glm::glm stb)

## Include Assets & Shaders ##
set(BUILD_PATH ${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR})
set(EXECUTABLE_OUTPUT_PATH ${BUILD_PATH})

set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
add_custom_target(CopyAssets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/${BUILD_PATH}/assets
        DEPENDS ${ASSETS_DIR}
)

add_dependencies(Haus CopyAssets)
message("-- Assets " ${ASSETS_DIR})

file(GLOB SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${BUILD_PATH}/shaders)
message("-- Shaders "  ${CMAKE_BINARY_DIR}/${BUILD_PATH}/shaders)

foreach (SHADER_PATH ${SHADERS})
    get_filename_component(SHADER_EXT ${SHADER_PATH} EXT)
    string(SUBSTRING ${SHADER_EXT} 1 -1 SHADER_EXTENSION)

    set(OUTPUT_PATH ${CMAKE_BINARY_DIR}/${BUILD_PATH}/shaders/${SHADER_EXTENSION}.spv)

    add_custom_command(
            OUTPUT ${OUTPUT_PATH}
            COMMAND ${CMAKE_COMMAND} -E echo "Compiling shader: ${SHADER_PATH}"
            COMMAND glslc ${SHADER_PATH} -o ${OUTPUT_PATH}
            DEPENDS ${SHADER_PATH}
            COMMENT "Compiling shader ${SHADER_PATH}"
    )

    list(APPEND GENERATED_SHADERS ${OUTPUT_PATH})
endforeach ()

add_custom_target(GenerateShaders DEPENDS ${GENERATED_SHADERS})

add_dependencies(${PROJECT_NAME} GenerateShaders)
