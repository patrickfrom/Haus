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

        static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

        GLFWwindow* m_Window;
    private:
        const int MAX_FRAMES_IN_FLIGHT = 2;
        uint32_t m_CurrentFrame = 0;

        SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device);
        vk::ShaderModule CreateShaderModule(const std::vector<char>& code);
        void RecordCommandBuffer(vk::CommandBuffer commandBuffer, uint32_t imageIndex);
        void DrawFrame();

        void CleanupSwapchain();
        void RecreateSwapchain();

        void InitVulkan();
        void CreateInstance();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapchain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateGraphicsPipeline();
        void CreateFramebuffers();
        void CreateCommandPool();
        void CreateVertexBuffer();
        void CreateCommandBuffers();
        void CreateSyncObjects();

        void CleanupVulkan();

        vk::Instance m_Instance;
        vk::SurfaceKHR m_Surface;
        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_Device;

        vk::SwapchainKHR m_Swapchain;
        std::vector<vk::Image> m_SwapchainImages;
        std::vector<vk::ImageView> m_SwapchainImageViews;
        vk::Format m_SwapchainImageFormat;
        vk::Extent2D m_SwapchainExtent;
        std::vector<vk::Framebuffer> m_SwapchainFramebuffers;

        vk::RenderPass m_RenderPass;

        vk::PipelineLayout m_PipelineLayout;
        vk::Pipeline m_GraphicsPipeline;

        vk::CommandPool m_CommandPool;
        std::vector<vk::CommandBuffer> m_CommandBuffers;

        std::vector<vk::Semaphore> m_ImageAvailableSemaphores;
        std::vector<vk::Semaphore> m_RenderFinishedSemaphores;
        std::vector<vk::Fence> m_InFlightFences;

        vk::Buffer m_VertexBuffer;

        bool m_FramebufferResized = false;

        vk::Queue m_GraphicsQueue;
    };

} // Haus
