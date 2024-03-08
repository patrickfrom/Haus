#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "glm/vec4.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/hash.hpp>
#include "Vulkan/VulkanContext.h"
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TextureCoord;
    glm::vec3 Normal;

    static vk::VertexInputBindingDescription GetBindingDescription() {
        vk::VertexInputBindingDescription bindingDescription{
                .binding = 0,
                .stride = sizeof(Vertex),
                .inputRate = vk::VertexInputRate::eVertex
        };

        return bindingDescription;
    }

    static std::array<vk::VertexInputAttributeDescription, 4> GetAttributeDescriptions() {
        std::array<vk::VertexInputAttributeDescription, 4> attributeDescription{
                vk::VertexInputAttributeDescription{
                        .location = 0,
                        .binding = 0,
                        .format = vk::Format::eR32G32B32Sfloat,
                        .offset = offsetof(Vertex, Position)
                },
                vk::VertexInputAttributeDescription{
                        .location = 1,
                        .binding = 0,
                        .format = vk::Format::eR32G32B32Sfloat,
                        .offset = offsetof(Vertex, Color)
                },
                vk::VertexInputAttributeDescription{
                        .location = 2,
                        .binding = 0,
                        .format = vk::Format::eR32G32Sfloat,
                        .offset = offsetof(Vertex, TextureCoord)
                },
                vk::VertexInputAttributeDescription{
                        .location = 3,
                        .binding = 0,
                        .format = vk::Format::eR32G32B32Sfloat,
                        .offset = offsetof(Vertex, Normal)
                }
        };

        return attributeDescription;
    }

    bool operator==(const Vertex &other) const {
        return Position == other.Position && Color == other.Color && TextureCoord == other.TextureCoord;
    }
};

namespace std {
    template<>
    struct hash<Vertex> {
        size_t operator()(Vertex const &vertex) const {
            return ((hash<glm::vec3>()(vertex.Position) ^
                     (hash<glm::vec3>()(vertex.Color) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.TextureCoord) << 1);
        }
    };
}

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
        std::vector<bool> m_MsaaChanged;
        const int MAX_FRAMES_IN_FLIGHT = 2;
        uint32_t m_CurrentFrame = 0;

        uint32_t m_MinImageCount;

        vk::ImageView
        CreateImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels);

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

        void LoadModel();

        void CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);

        void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::SampleCountFlagBits numSamples,
                         vk::Format format, vk::ImageTiling tiling,
                         vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image &image,
                         vk::DeviceMemory &imageMemory);

        void
        TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                              uint32_t mipLevels);

        void GenerateMipmaps(vk::Image image, int32_t width, int32_t height, uint32_t mipLevels);

        void CleanupSwapchain();

        void RecreateSwapchain();

        void InitVulkan();

        void CreateSurface();


        void CreateLogicalDevice();

        void CreateSwapchain();

        void CreateImageViews();

        void CreateRenderPass();

        void CreateDescriptorSetLayout();

        void CreateGraphicsPipeline();

        void CreateWireframePipeline(vk::GraphicsPipelineCreateInfo &pipelineInfo,
                                     vk::PipelineRasterizationStateCreateInfo &rasterizer);

        void CreateFramebuffers();

        void CreateCommandPool();

        void CreateColorResources();

        void CreateDepthResources();

        void CreateTextureImage();

        void CreateTextureImageView();

        void CreateTextureSampler();

        void CreateVertexBuffer();

        void CreateIndexBuffer();

        void CreateUniformBuffers();

        void CreateDescriptorPool();

        void CreateDescriptorSets();

        void CreateCommandBuffers();

        void CreateSyncObjects();

        void CleanupVulkan();

        vk::SampleCountFlagBits GetMaxUsableSampleCount();

        bool m_WireframeEnabled = false;

        VulkanContext* m_VulkanContext{};
        vk::SurfaceKHR m_Surface;
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


        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        vk::Buffer m_VertexBuffer;
        vk::DeviceMemory m_VertexBufferMemory;

        vk::Buffer m_IndexBuffer;
        vk::DeviceMemory m_IndexBufferMemory;

        std::vector<vk::Buffer> m_UniformBuffers;
        std::vector<vk::DeviceMemory> m_UniformBuffersMemory;
        std::vector<void *> m_UniformBuffersMapped;

        vk::DescriptorPool m_DescriptorPool;
        std::vector<vk::DescriptorSet> m_DescriptorSets;

        vk::SampleCountFlagBits m_MsaaSamples = vk::SampleCountFlagBits::e1;

        uint32_t m_MipLevels;
        vk::Image m_TextureImage;
        vk::ImageView m_TextureImageView;
        vk::Sampler m_TextureSampler;
        vk::DeviceMemory m_TextureImageMemory;

        vk::Image m_ColorImage;
        vk::ImageView m_ColorImageView;
        vk::DeviceMemory m_ColorImageMemory;

        vk::Image m_DepthImage;
        vk::ImageView m_DepthImageView;
        vk::DeviceMemory m_DepthImageMemory;

        vk::ClearValue m_ClearColor = {{{{0.0f, 0.0f, 0.0f, 1.0}}}};

        bool m_FramebufferResized = false;

        vk::Queue m_GraphicsQueue;
    };

} // Haus
