cmake_minimum_required(VERSION 3.27)
project(HausEngine)

set(CMAKE_CXX_STANDARD 20)

add_library(HausEngine
        Core/Application.cpp
        Core/Application.h
        HausEngine.h
        vendors/stb/image.h
        vendors/tiny_obj_loader.h
        Core/Window.cpp
        Core/Window.h
        Vulkan/VulkanContext.h
        Vulkan/VulkanContext.cpp
        Vulkan/VulkanDevice.h
        Vulkan/VulkanDevice.cpp
        Core/Ref.h
        Vulkan/VulkanSwapchain.h
        Vulkan/VulkanSwapchain.cpp)

target_include_directories(HausEngine PUBLIC Vulkan Core)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(vendors/GLFW)
add_subdirectory(vendors/glm)
add_subdirectory(vendors/stb)

find_package(Vulkan REQUIRED)
target_link_libraries(HausEngine PUBLIC Vulkan::Vulkan glfw glm::glm stb)