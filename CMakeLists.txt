cmake_minimum_required(VERSION 3.27)
project(Haus)

set(CMAKE_CXX_STANDARD 20)

add_executable(Haus main.cpp
        Application.cpp
        Application.h
        vendors/stb/image.h)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(vendors/GLFW)
add_subdirectory(vendors/glm)
add_subdirectory(vendors/stb)

find_package(Vulkan REQUIRED)


target_link_libraries(Haus PRIVATE Vulkan::Vulkan glfw glm::glm stb)