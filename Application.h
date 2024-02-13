#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>

namespace Haus {
    struct ApplicationSpecification {
        std::string Name;
        int Width;
        int Height;
    };

    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR Capabilities;
        std::vector<vk::SurfaceFormatKHR> Formats;
        std::vector<vk::PresentModeKHR> PresentModes;
    };

    class Application {
    public:
        explicit Application(ApplicationSpecification& specification);

        void Run();
        void Shutdown();
    private:
        ApplicationSpecification m_Specification;

        void Loop();
    private:
        void InitWindow();
        void CleanupGLFW();

        GLFWwindow* m_Window;
    private:
        SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device);

        void InitVulkan();
        void CreateInstance();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapchain();

        void CleanupVulkan();

        vk::Instance m_Instance;
        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_Device;

        vk::SurfaceKHR m_Surface;
        vk::SwapchainKHR m_Swapchain;
        std::vector<vk::Image> m_SwapchainImages;
        vk::Format m_SwapchainImageFormat;
        vk::Extent2D m_SwapchainExtent;

        vk::Queue m_GraphicsQueue;
    };

} // Haus
