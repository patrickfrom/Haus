#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "glm/vec4.hpp"

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
        explicit Application(ApplicationSpecification &specification);

        void Run();

        void Shutdown();

    private:
        ApplicationSpecification m_Specification;

        void Loop();

    private:
        void InitWindow();

        void CleanupGLFW();

        static void FramebufferResizeCallback(GLFWwindow *window, int width, int height);
        static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

        GLFWwindow *m_Window;

    public:
        void SetClearColor(glm::vec4 color) {
            m_ClearColor = {{{{color.x, color.y, color.z, color.w}}}};
        }

    private:
        const int MAX_FRAMES_IN_FLIGHT = 2;
        uint32_t m_CurrentFrame = 0;

        uint32_t m_MinImageCount;

        SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device);

        vk::ShaderModule CreateShaderModule(const std::vector<char> &code);

        void RecordCommandBuffer(vk::CommandBuffer commandBuffer, uint32_t imageIndex);

        void DrawFrame();

        void UpdateUniformBuffer(uint32_t currentImage);

        uint32_t FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

        void CreateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
                          vk::Buffer &buffer, vk::DeviceMemory &bufferMemory);

        void CopyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);

        vk::CommandBuffer BeginSingleTimeCommands();

        void EndSingleTimeCommands(vk::CommandBuffer commandBuffer);

        void CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);

        void CreateImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
                         vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image &image,
                         vk::DeviceMemory &imageMemory);

        void TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);

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

        void CreateDescriptorSetLayout();

        void CreateGraphicsPipeline();
        void CreateWireframePipeline(vk::GraphicsPipelineCreateInfo& pipelineInfo, vk::PipelineRasterizationStateCreateInfo& rasterizer);

        void CreateFramebuffers();

        void CreateCommandPool();

        void CreateTextureImage();

        void CreateVertexBuffer();

        void CreateIndexBuffer();

        void CreateUniformBuffers();

        void CreateDescriptorPool();

        void CreateDescriptorSets();

        void CreateCommandBuffers();

        void CreateSyncObjects();

        void CleanupVulkan();

        bool m_WireframeEnabled = false;

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

        vk::DescriptorSetLayout m_DescriptorSetLayout;
        vk::PipelineLayout m_PipelineLayout;
        vk::Pipeline m_GraphicsPipeline;
        vk::PipelineCache m_GraphicsPipelineCache;
        vk::Pipeline m_WireframePipeline;

        vk::CommandPool m_CommandPool;
        std::vector<vk::CommandBuffer> m_CommandBuffers;

        std::vector<vk::Semaphore> m_ImageAvailableSemaphores;
        std::vector<vk::Semaphore> m_RenderFinishedSemaphores;
        std::vector<vk::Fence> m_InFlightFences;

        vk::Buffer m_VertexBuffer;
        vk::DeviceMemory m_VertexBufferMemory;

        vk::Buffer m_IndexBuffer;
        vk::DeviceMemory m_IndexBufferMemory;

        std::vector<vk::Buffer> m_UniformBuffers;
        std::vector<vk::DeviceMemory> m_UniformBuffersMemory;
        std::vector<void *> m_UniformBuffersMapped;

        vk::DescriptorPool m_DescriptorPool;
        std::vector<vk::DescriptorSet> m_DescriptorSets;

        vk::Image m_CatImage;
        vk::DeviceMemory m_CatImageMemory;

        vk::ClearValue m_ClearColor = {{{{0.0f, 0.0f, 0.0f, 1.0}}}};

        bool m_FramebufferResized = false;

        vk::Queue m_GraphicsQueue;
    };

} // Haus
