#include "Application.h"
#include <iostream>
#include <format>
#include <fstream>
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION

#include <stb/image.h>

#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>

#include <unordered_map>

/* Currently using regions, just so it's easier for me to
   see what's going on since I don't want to abstract
   Vulkan or GLFW until I understand them a little better*/
namespace Haus {
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat3 normalInverse;
    };


    /*const std::vector<Vertex> vertices = {
            // Front face
            {{0.5f,  0.5f,  0.5f},  {1.0f, 0.0f,  0.0f},  {1.0f, 1.0f}, {0.0f,  0.0f,  1.0f}}, // 0
            {{0.5f,  -0.5f, 0.5f},  {1.0f, 0.32f, 0.32f}, {1.0f, 0.0f}, {0.0f,  0.0f,  1.0f}}, // 1
            {{-0.5f, -0.5f, 0.5f},  {1.0f, 0.29f, 0.14f}, {0.0f, 0.0f}, {0.0f,  0.0f,  1.0f}}, // 2
            {{-0.5f, 0.5f,  0.5f},  {1.0f, 0.32f, 0.32f}, {0.0f, 1.0f}, {0.0f,  0.0f,  1.0f}}, // 3

            // Back face
            {{0.5f,  0.5f,  -0.5f}, {1.0f, 0.0f,  0.0f},  {1.0f, 1.0f}, {0.0f,  0.0f,  -1.0f}}, // 4
            {{0.5f,  -0.5f, -0.5f}, {1.0f, 0.32f, 0.32f}, {1.0f, 0.0f}, {0.0f,  0.0f,  -1.0f}}, // 5
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.29f, 0.14f}, {0.0f, 0.0f}, {0.0f,  0.0f,  -1.0f}}, // 6
            {{-0.5f, 0.5f,  -0.5f}, {1.0f, 0.32f, 0.32f}, {0.0f, 1.0f}, {0.0f,  0.0f,  -1.0f}}, // 7

            // Right face
            {{0.5f,  0.5f,  0.5f},  {1.0f, 0.0f,  0.0f},  {1.0f, 1.0f}, {1.0f,  0.0f,  0.0f}}, // 8
            {{0.5f,  -0.5f, 0.5f},  {1.0f, 0.32f, 0.32f}, {1.0f, 0.0f}, {1.0f,  0.0f,  0.0f}}, // 9
            {{0.5f,  -0.5f, -0.5f}, {1.0f, 0.29f, 0.14f}, {0.0f, 0.0f}, {1.0f,  0.0f,  0.0f}}, // 10
            {{0.5f,  0.5f,  -0.5f}, {1.0f, 0.32f, 0.32f}, {0.0f, 1.0f}, {1.0f,  0.0f,  0.0f}}, // 11

            // Left face
            {{-0.5f, 0.5f,  0.5f},  {1.0f, 0.0f,  0.0f},  {1.0f, 1.0f}, {-1.0f, 0.0f,  0.0f}}, // 12
            {{-0.5f, -0.5f, 0.5f},  {1.0f, 0.32f, 0.32f}, {1.0f, 0.0f}, {-1.0f, 0.0f,  0.0f}}, // 13
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.29f, 0.14f}, {0.0f, 0.0f}, {-1.0f, 0.0f,  0.0f}}, // 14
            {{-0.5f, 0.5f,  -0.5f}, {1.0f, 0.32f, 0.32f}, {0.0f, 1.0f}, {-1.0f, 0.0f,  0.0f}}, // 15

            // Top face
            {{0.5f,  0.5f,  0.5f},  {1.0f, 0.0f,  0.0f},  {1.0f, 1.0f}, {0.0f,  1.0f,  0.0f}}, // 16
            {{0.5f,  0.5f,  -0.5f}, {1.0f, 0.32f, 0.32f}, {1.0f, 0.0f}, {0.0f,  1.0f,  0.0f}}, // 17
            {{-0.5f, 0.5f,  -0.5f}, {1.0f, 0.29f, 0.14f}, {0.0f, 0.0f}, {0.0f,  1.0f,  0.0f}}, // 18
            {{-0.5f, 0.5f,  0.5f},  {1.0f, 0.32f, 0.32f}, {0.0f, 1.0f}, {0.0f,  1.0f,  0.0f}}, // 19

            // Bottom face
            {{0.5f,  -0.5f, 0.5f},  {1.0f, 0.0f,  0.0f},  {1.0f, 1.0f}, {0.0f,  -1.0f, 0.0f}}, // 20
            {{0.5f,  -0.5f, -0.5f}, {1.0f, 0.32f, 0.32f}, {1.0f, 0.0f}, {0.0f,  -1.0f, 0.0f}}, // 21
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.29f, 0.14f}, {0.0f, 0.0f}, {0.0f,  -1.0f, 0.0f}}, // 22
            {{-0.5f, -0.5f, 0.5f},  {1.0f, 0.32f, 0.32f}, {0.0f, 1.0f}, {0.0f,  -1.0f, 0.0f}}  // 23
    };

    const std::vector<uint16_t> indices{
            // Front face
            0, 1, 2,
            2, 3, 0,

            // Back face
            6, 5, 4,
            4, 7, 6,

            // Right face
            10, 9, 8,
            8, 11, 10,

            // Left face
            12, 13, 14,
            14, 15, 12,

            // Top face
            18, 17, 16,
            16, 19, 18,

            // Bottom face
            20, 21, 22,
            22, 23, 20
    };*/

#pragma region APPLICATION

    Application::Application(ApplicationSpecification &specification) : m_Specification(specification) {}

    void Application::Run() {
        std::cout << std::format("Starting Application {}", m_Specification.Name) << "\n";

        InitWindow();
        InitVulkan();
        Loop();
    }

    void Application::Shutdown() {
        std::cout << std::format("Closing Application {}", m_Specification.Name) << "\n";

        CleanupVulkan();
        CleanupGLFW();
    }

    void Application::Loop() {
        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();
            DrawFrame();
        }

        m_Device.waitIdle();
    }

#pragma endregion APPLICATION

#pragma region GLFW

    void Application::InitWindow() {
        if (!glfwInit())
            throw std::runtime_error("Failed to Initialize GLFW");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_Window = glfwCreateWindow(m_Specification.Width, m_Specification.Height, m_Specification.Name.c_str(),
                                    nullptr, nullptr);
        if (!m_Window)
            throw std::runtime_error("Failed to create GLFW Window");

        glfwSetWindowUserPointer(m_Window, this);
        glfwSetFramebufferSizeCallback(m_Window, FramebufferResizeCallback);
        glfwSetKeyCallback(m_Window, KeyCallback);
    }

    void Application::FramebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

        app->m_FramebufferResized = true;
    }

    void Application::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
        if (key == GLFW_KEY_E && action == GLFW_RELEASE)
            app->m_WireframeEnabled = !app->m_WireframeEnabled;
    }

    void Application::CleanupGLFW() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

#pragma endregion GLFW

#pragma region VULKAN

    void Application::InitVulkan() {
        std::cout << "Initializing Vulkan" << "\n";
        CreateInstance();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapchain();
        CreateImageViews();
        CreateRenderPass();
        CreateDescriptorSetLayout();
        CreateGraphicsPipeline();
        CreateCommandPool();
        CreateDepthResources();
        CreateFramebuffers();
        CreateTextureImage();
        CreateTextureImageView();
        CreateTextureSampler();
        LoadModel();
        CreateVertexBuffer();
        CreateIndexBuffer();
        CreateUniformBuffers();
        CreateDescriptorPool();
        CreateDescriptorSets();
        CreateCommandBuffers();
        CreateSyncObjects();
    }

    void Application::LoadModel() {
        std::string modelFile = "models/Moon/Moon 2K.obj";
        tinyobj::ObjReader reader;

        tinyobj::ObjReaderConfig config{};
        config.triangulate = true;
        config.mtl_search_path = "models/Moon";

        if (!reader.ParseFromFile(modelFile, config)) {
            if (!reader.Error().empty()) {
                std::cerr << "TinyObjReader: " << reader.Error();
            }
            exit(1);
        }

        if (!reader.Warning().empty()) {
            std::cout << "TinyObjReader: " << reader.Warning();
        }

        auto &attrib = reader.GetAttrib();
        auto &shapes = reader.GetShapes();
        auto &materials = reader.GetMaterials();

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (const auto &shape: shapes) {
            for (const auto &index: shape.mesh.indices) {
                Vertex vertex{};
                vertex.Position = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2],
                };

                vertex.Normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2],
                };

                vertex.TextureCoord = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1],
                };

                vertex.Color = {1.0f, 1.0f, 1.0f};

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }

    void Application::CreateInstance() {
        vk::ApplicationInfo appInfo{
                .pApplicationName = m_Specification.Name.c_str(),
                .applicationVersion = VK_MAKE_VERSION(1, 0, 0),

                .pEngineName = "No Engine",
                .engineVersion = VK_MAKE_VERSION(1, 0, 0),

                .apiVersion = VK_API_VERSION_1_3
        };

        uint32_t glfwExtensionCount = 0;
        const char **extensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        vk::InstanceCreateInfo createInfo{
                .pApplicationInfo = &appInfo,
                .enabledExtensionCount = glfwExtensionCount,
                .ppEnabledExtensionNames = extensions
        };

        vk::Result result = vk::createInstance(&createInfo, nullptr, &m_Instance);
        if (result != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create Instance");
    }

    void Application::CreateSurface() {
        if (glfwCreateWindowSurface(m_Instance, m_Window, nullptr, reinterpret_cast<VkSurfaceKHR *>(&m_Surface)))
            throw std::runtime_error("Failed to create window surface!");
    }

    bool IsDeviceSuitable(vk::PhysicalDevice device) {
        vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
        vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();
        return deviceFeatures.geometryShader && deviceFeatures.fillModeNonSolid && deviceFeatures.samplerAnisotropy;
    }

    void Application::PickPhysicalDevice() {

        std::vector<vk::PhysicalDevice> devices = m_Instance.enumeratePhysicalDevices();
        if (devices.empty())
            throw std::runtime_error("Failed to find GPUs with vulkan support");

        for (const auto &device: devices) {
            if (IsDeviceSuitable(device)) {
                m_PhysicalDevice = device;
                break;
            }
        }

        if (m_PhysicalDevice == VK_NULL_HANDLE)
            throw std::runtime_error("Failed to find a suitable GPU!");

        vk::PhysicalDeviceProperties deviceProperties = m_PhysicalDevice.getProperties();
        std::cout << deviceProperties.deviceName << "\n";
        std::cout << to_string(deviceProperties.deviceType) << "\n";
    }

    void Application::CreateLogicalDevice() {
        float priority = 1.0f;
        vk::DeviceQueueCreateInfo queueCreateInfo{
                .queueFamilyIndex = 0,
                .queueCount = 1,
                .pQueuePriorities = &priority,
        };

        vk::PhysicalDeviceFeatures deviceFeatures{
                .fillModeNonSolid = vk::True,
                .samplerAnisotropy = vk::True
        };

        std::vector<const char *> enabledExtensions = {"VK_KHR_swapchain"};

        vk::DeviceCreateInfo createInfo{
                .queueCreateInfoCount = 1,
                .pQueueCreateInfos = &queueCreateInfo,

                .enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size()),
                .ppEnabledExtensionNames = enabledExtensions.data(),

                .pEnabledFeatures = &deviceFeatures,
        };

        m_Device = m_PhysicalDevice.createDevice(createInfo);
        if (!m_Device)
            throw std::runtime_error("Failed to create logical device");

        m_GraphicsQueue = m_Device.getQueue(0, 0);
    }

    SwapChainSupportDetails Application::QuerySwapChainSupport(vk::PhysicalDevice device) {
        SwapChainSupportDetails details{
                .Capabilities = device.getSurfaceCapabilitiesKHR(m_Surface),
                .Formats = device.getSurfaceFormatsKHR(m_Surface),
                .PresentModes = device.getSurfacePresentModesKHR(m_Surface)
        };

        return details;
    }

    void Application::CreateSwapchain() {
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_PhysicalDevice);

        vk::Extent2D extent{};
        if (swapChainSupport.Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            extent = swapChainSupport.Capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(m_Window, &width, &height);

            extent.width = width;
            extent.height = height;

            extent.width = std::clamp(extent.width, swapChainSupport.Capabilities.minImageExtent.width,
                                      swapChainSupport.Capabilities.maxImageExtent.width);
            extent.height = std::clamp(extent.height, swapChainSupport.Capabilities.minImageExtent.height,
                                       swapChainSupport.Capabilities.maxImageExtent.height);
        }

        uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;
        if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
            imageCount = swapChainSupport.Capabilities.maxImageCount;

        m_MinImageCount = imageCount;

        vk::SwapchainCreateInfoKHR createInfo{
                .surface = m_Surface,
                .minImageCount = imageCount,
                .imageFormat = swapChainSupport.Formats[0].format,
                .imageColorSpace = swapChainSupport.Formats[0].colorSpace,
                .imageExtent = extent,
                .imageArrayLayers = 1,
                .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
                .imageSharingMode = vk::SharingMode::eExclusive,
                .preTransform = swapChainSupport.Capabilities.currentTransform,
                .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
                .presentMode = vk::PresentModeKHR::eMailbox,
                .clipped = VK_TRUE,
        };

        if (m_Device.createSwapchainKHR(&createInfo, nullptr, &m_Swapchain) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create swap chain");

        m_SwapchainImages = m_Device.getSwapchainImagesKHR(m_Swapchain);

        m_SwapchainImageFormat = swapChainSupport.Formats[0].format;
        m_SwapchainExtent = extent;
    }

    void Application::CleanupSwapchain() {
        m_Device.destroyImageView(m_DepthImageView);
        m_Device.destroyImage(m_DepthImage);
        m_Device.freeMemory(m_DepthImageMemory);

        for (auto framebuffer: m_SwapchainFramebuffers)
            m_Device.destroyFramebuffer(framebuffer);

        for (auto imageView: m_SwapchainImageViews)
            m_Device.destroyImageView(imageView);

        m_Device.destroySwapchainKHR(m_Swapchain);
    }

    void Application::RecreateSwapchain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(m_Window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(m_Window, &width, &height);
            glfwWaitEvents();
        }
        m_Device.waitIdle();

        CleanupSwapchain();

        CreateSwapchain();
        CreateImageViews();
        CreateDepthResources();
        CreateFramebuffers();
    }

    void Application::CreateImageViews() {
        m_SwapchainImageViews.resize(m_SwapchainImages.size());

        for (size_t i = 0; i < m_SwapchainImages.size(); i++) {
            m_SwapchainImageViews[i] = CreateImageView(m_SwapchainImages[i], m_SwapchainImageFormat,
                                                       vk::ImageAspectFlagBits::eColor, 1);
        }
    }

    void Application::CreateRenderPass() {
        vk::AttachmentDescription colorAttachment{
                .format = m_SwapchainImageFormat,
                .samples = vk::SampleCountFlagBits::e1,
                .loadOp = vk::AttachmentLoadOp::eClear,
                .storeOp = vk::AttachmentStoreOp::eStore,
                .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
                .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
                .initialLayout = vk::ImageLayout::eUndefined,
                .finalLayout = vk::ImageLayout::ePresentSrcKHR
        };

        vk::AttachmentReference colorAttachmentReference{
                .attachment = 0,
                .layout = vk::ImageLayout::eColorAttachmentOptimal
        };

        vk::AttachmentDescription depthAttachment{
                .format = vk::Format::eD32Sfloat,
                .samples = vk::SampleCountFlagBits::e1,
                .loadOp = vk::AttachmentLoadOp::eClear,
                .storeOp = vk::AttachmentStoreOp::eDontCare,
                .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
                .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
                .initialLayout = vk::ImageLayout::eUndefined,
                .finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal
        };

        vk::AttachmentReference depthAttachmentReference{
                .attachment = 1,
                .layout = vk::ImageLayout::eDepthStencilAttachmentOptimal
        };

        vk::SubpassDescription subpass{
                .pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
                .colorAttachmentCount = 1,
                .pColorAttachments = &colorAttachmentReference,
                .pDepthStencilAttachment = &depthAttachmentReference
        };

        vk::SubpassDependency dependency{
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput |
                                vk::PipelineStageFlagBits::eEarlyFragmentTests,
                .dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput |
                                vk::PipelineStageFlagBits::eEarlyFragmentTests,
                .srcAccessMask = vk::AccessFlagBits::eNone,
                .dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite |
                                 vk::AccessFlagBits::eDepthStencilAttachmentWrite
        };

        std::array<vk::AttachmentDescription, 2> attachments = {
                colorAttachment, depthAttachment
        };

        vk::RenderPassCreateInfo renderPassInfo{
                .attachmentCount = static_cast<uint32_t>(attachments.size()),
                .pAttachments = attachments.data(),
                .subpassCount = 1,
                .pSubpasses = &subpass,
                .dependencyCount = 1,
                .pDependencies = &dependency
        };

        if (m_Device.createRenderPass(&renderPassInfo, nullptr, &m_RenderPass) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create render pass");
    }

    static std::vector<char> ReadFile(const std::string &filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
            throw std::runtime_error("Failed to open file");

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    vk::ShaderModule Application::CreateShaderModule(const std::vector<char> &code) {
        vk::ShaderModuleCreateInfo createInfo{
                .codeSize = code.size(),
                .pCode = reinterpret_cast<const uint32_t *>(code.data())
        };

        vk::ShaderModule shaderModule;
        if (m_Device.createShaderModule(&createInfo, nullptr, &shaderModule) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create shader module!");

        return shaderModule;
    }

    void Application::CreateDescriptorSetLayout() {
        vk::DescriptorSetLayoutBinding uboLayoutBinding{
                .binding = 0,
                .descriptorType = vk::DescriptorType::eUniformBuffer,
                .descriptorCount = 1,
                .stageFlags = vk::ShaderStageFlagBits::eVertex
        };

        vk::DescriptorSetLayoutBinding samplerLayoutBinding{
                .binding = 1,
                .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                .descriptorCount = 1,
                .stageFlags = vk::ShaderStageFlagBits::eFragment
        };

        std::array<vk::DescriptorSetLayoutBinding, 2> bindings = {
                uboLayoutBinding, samplerLayoutBinding
        };

        vk::DescriptorSetLayoutCreateInfo layoutInfo{
                .bindingCount = static_cast<uint32_t>(bindings.size()),
                .pBindings = bindings.data()
        };

        if (m_Device.createDescriptorSetLayout(&layoutInfo, nullptr, &m_DescriptorSetLayout) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create descriptor set layout");
    }

    void Application::CreateGraphicsPipeline() {
        auto vertexShaderCode = ReadFile("vert.spv");
        auto fragmentShaderCode = ReadFile("frag.spv");

        vk::ShaderModule vertexShaderModule = CreateShaderModule(vertexShaderCode);
        vk::ShaderModule fragmentShaderModule = CreateShaderModule(fragmentShaderCode);

        vk::PipelineShaderStageCreateInfo vertexShaderStageInfo{
                .stage = vk::ShaderStageFlagBits::eVertex,
                .module = vertexShaderModule,
                .pName = "main"
        };

        vk::PipelineShaderStageCreateInfo fragmentShaderStageInfo{
                .stage = vk::ShaderStageFlagBits::eFragment,
                .module = fragmentShaderModule,
                .pName = "main"
        };

        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages = {
                vertexShaderStageInfo,
                fragmentShaderStageInfo
        };

        std::vector<vk::DynamicState> dynamicStates = {
                vk::DynamicState::eViewport,
                vk::DynamicState::eScissor
        };

        vk::PipelineDynamicStateCreateInfo dynamicState{
                .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
                .pDynamicStates = dynamicStates.data()
        };

        auto bindingDescription = Vertex::GetBindingDescription();
        auto attributeDescriptions = Vertex::GetAttributeDescriptions();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
                .vertexBindingDescriptionCount = 1,
                .pVertexBindingDescriptions = &bindingDescription,
                .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
                .pVertexAttributeDescriptions = attributeDescriptions.data()
        };

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
                .topology = vk::PrimitiveTopology::eTriangleList,
                .primitiveRestartEnable = VK_FALSE
        };

        vk::PipelineViewportStateCreateInfo viewportState{
                .viewportCount = 1,
                .scissorCount = 1,
        };

        vk::PipelineRasterizationStateCreateInfo rasterizer{
                .depthClampEnable = VK_FALSE,
                .rasterizerDiscardEnable = VK_FALSE,
                .polygonMode = vk::PolygonMode::eFill,
                .cullMode = vk::CullModeFlagBits::eBack,
                .frontFace = vk::FrontFace::eCounterClockwise,
                .depthBiasEnable = VK_FALSE,
                .lineWidth = 1.0f
        };

        vk::PipelineMultisampleStateCreateInfo multisampling{
                .rasterizationSamples = vk::SampleCountFlagBits::e1,
                .sampleShadingEnable = VK_FALSE,
        };

        vk::PipelineColorBlendAttachmentState colorBlendAttachment{
                .blendEnable = VK_TRUE,
                .srcColorBlendFactor = vk::BlendFactor::eSrcAlpha,
                .dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
                .colorBlendOp = vk::BlendOp::eAdd,
                .srcAlphaBlendFactor = vk::BlendFactor::eOne,
                .dstAlphaBlendFactor = vk::BlendFactor::eZero,
                .alphaBlendOp = vk::BlendOp::eAdd,
                .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG
                                  | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
        };

        vk::PipelineColorBlendStateCreateInfo colorBlending{
                .logicOpEnable = VK_FALSE,
                .logicOp = vk::LogicOp::eCopy,
                .attachmentCount = 1,
                .pAttachments = &colorBlendAttachment
        };

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo{
                .setLayoutCount = 1,
                .pSetLayouts = &m_DescriptorSetLayout
        };

        vk::PipelineDepthStencilStateCreateInfo depthStencil{
                .depthTestEnable = vk::True,
                .depthWriteEnable = vk::True,
                .depthCompareOp = vk::CompareOp::eLess,
                .depthBoundsTestEnable = vk::False,
                .stencilTestEnable = vk::False,
                .minDepthBounds = 0.0f,
                .maxDepthBounds = 1.0f
        };

        if (m_Device.createPipelineLayout(&pipelineLayoutInfo, nullptr, &m_PipelineLayout) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create pipeline layout!");

        vk::GraphicsPipelineCreateInfo pipelineInfo{
                .flags = vk::PipelineCreateFlagBits::eAllowDerivatives,
                .stageCount = static_cast<uint32_t>(shaderStages.size()),
                .pStages = shaderStages.data(),
                .pVertexInputState = &vertexInputInfo,
                .pInputAssemblyState = &inputAssembly,
                .pViewportState = &viewportState,
                .pRasterizationState = &rasterizer,
                .pMultisampleState = &multisampling,
                .pDepthStencilState = &depthStencil,
                .pColorBlendState = &colorBlending,
                .pDynamicState = &dynamicState,
                .layout = m_PipelineLayout,
                .renderPass = m_RenderPass,
                .subpass = 0
        };

        if (m_Device.createGraphicsPipelines(m_GraphicsPipelineCache, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) !=
            vk::Result::eSuccess)
            throw std::runtime_error("Failed to create Graphics Pipeline");

        CreateWireframePipeline(pipelineInfo, rasterizer);

        m_Device.destroyShaderModule(vertexShaderModule);
        m_Device.destroyShaderModule(fragmentShaderModule);
    }

    void Application::CreateWireframePipeline(vk::GraphicsPipelineCreateInfo &pipelineInfo,
                                              vk::PipelineRasterizationStateCreateInfo &rasterizer) {
        rasterizer.polygonMode = vk::PolygonMode::eLine;

        pipelineInfo.flags = vk::PipelineCreateFlagBits::eDerivative;
        pipelineInfo.basePipelineHandle = m_GraphicsPipeline;
        pipelineInfo.basePipelineIndex = -1;

        if (m_Device.createGraphicsPipelines(m_GraphicsPipelineCache, 1, &pipelineInfo, nullptr,
                                             &m_WireframePipeline) !=
            vk::Result::eSuccess)
            throw std::runtime_error("Failed to create Wireframe Pipeline");
    }

    void Application::CreateFramebuffers() {
        m_SwapchainFramebuffers.resize(m_SwapchainImageViews.size());

        for (size_t i = 0; i < m_SwapchainImageViews.size(); i++) {
            std::array<vk::ImageView, 2> attachments = {
                    m_SwapchainImageViews[i],
                    m_DepthImageView
            };

            vk::FramebufferCreateInfo framebufferInfo{
                    .renderPass = m_RenderPass,
                    .attachmentCount = static_cast<uint32_t>(attachments.size()),
                    .pAttachments = attachments.data(),
                    .width = m_SwapchainExtent.width,
                    .height = m_SwapchainExtent.height,
                    .layers = 1
            };

            if (m_Device.createFramebuffer(&framebufferInfo, nullptr, &m_SwapchainFramebuffers[i]) !=
                vk::Result::eSuccess)
                throw std::runtime_error("Failed to create framebuffer");
        }
    }

    void Application::CreateCommandPool() {
        vk::CommandPoolCreateInfo poolInfo{
                .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                .queueFamilyIndex = 0
        };

        if (m_Device.createCommandPool(&poolInfo, nullptr, &m_CommandPool) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create command pool");
    }

    void Application::CreateDepthResources() {
        CreateImage(m_SwapchainExtent.width, m_SwapchainExtent.height, 1, vk::Format::eD32Sfloat,
                    vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment,
                    vk::MemoryPropertyFlagBits::eDeviceLocal, m_DepthImage, m_DepthImageMemory);
        m_DepthImageView = CreateImageView(m_DepthImage, vk::Format::eD32Sfloat, vk::ImageAspectFlagBits::eDepth, 1);

        TransitionImageLayout(m_DepthImage, vk::Format::eD32Sfloat, vk::ImageLayout::eUndefined,
                              vk::ImageLayout::eDepthStencilAttachmentOptimal, 1);
    }

    vk::CommandBuffer Application::BeginSingleTimeCommands() {
        vk::CommandBufferAllocateInfo allocateInfo{
                .commandPool = m_CommandPool,
                .level = vk::CommandBufferLevel::ePrimary,
                .commandBufferCount = 1
        };

        vk::CommandBuffer commandBuffer;
        if (m_Device.allocateCommandBuffers(&allocateInfo, &commandBuffer) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to allocate command buffers");

        vk::CommandBufferBeginInfo beginInfo{
                .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
        };

        commandBuffer.begin(beginInfo);

        return commandBuffer;
    }

    void Application::EndSingleTimeCommands(vk::CommandBuffer commandBuffer) {
        commandBuffer.end();

        vk::SubmitInfo submitInfo{
                .commandBufferCount = 1,
                .pCommandBuffers = &commandBuffer
        };

        m_GraphicsQueue.submit(1, &submitInfo, VK_NULL_HANDLE);
        m_GraphicsQueue.waitIdle();

        m_Device.freeCommandBuffers(m_CommandPool, 1, &commandBuffer);
    }

    void Application::CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height) {
        vk::CommandBuffer commandBuffer = BeginSingleTimeCommands();

        vk::BufferImageCopy region{
                .bufferOffset = 0,
                .bufferRowLength = 0,
                .bufferImageHeight = 0,
                .imageSubresource {
                        .aspectMask = vk::ImageAspectFlagBits::eColor,
                        .mipLevel = 0,
                        .baseArrayLayer = 0,
                        .layerCount = 1
                },
                .imageOffset {0, 0, 0},
                .imageExtent {
                        .width = width,
                        .height = height,
                        .depth = 1
                }
        };

        commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &region);

        EndSingleTimeCommands(commandBuffer);
    }

    void Application::CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::Format format,
                                  vk::ImageTiling tiling,
                                  vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image &image,
                                  vk::DeviceMemory &imageMemory) {
        vk::ImageCreateInfo imageInfo{
                .imageType = vk::ImageType::e2D,
                .format = format,
                .extent {
                        .width = static_cast<uint32_t>(width),
                        .height = static_cast<uint32_t>(height),
                        .depth = 1
                },
                .mipLevels = mipLevels,
                .arrayLayers = 1,
                .samples = vk::SampleCountFlagBits::e1,
                .tiling = tiling,
                .usage = usage,
                .sharingMode = vk::SharingMode::eExclusive,
                .initialLayout = vk::ImageLayout::eUndefined,
        };

        if (m_Device.createImage(&imageInfo, nullptr, &image) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create image");

        vk::MemoryRequirements memoryRequirements = m_Device.getImageMemoryRequirements(image);

        vk::MemoryAllocateInfo allocateInfo{
                .allocationSize = memoryRequirements.size,
                .memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits,
                                                  properties)
        };

        if (m_Device.allocateMemory(&allocateInfo, nullptr, &imageMemory) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to allocate image memory");

        m_Device.bindImageMemory(image, imageMemory, 0);
    }

    void Application::CreateTextureImage() {
        stbi_set_flip_vertically_on_load(true);
        int width, height, channels;
        stbi_uc *pixels = stbi_load("models/Moon/Textures/Diffuse_2K.png", &width, &height, &channels, STBI_rgb_alpha);
        m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        vk::DeviceSize imageSize = width * height * 4;

        if (!pixels)
            throw std::runtime_error("Failed to load texture image");

        vk::Buffer stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        CreateBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc,
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                     stagingBuffer, stagingBufferMemory);

        void *data = m_Device.mapMemory(stagingBufferMemory, 0, imageSize);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        m_Device.unmapMemory(stagingBufferMemory);

        stbi_image_free(pixels);

        CreateImage(width, height, m_MipLevels, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal,
                    vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst |
                    vk::ImageUsageFlagBits::eSampled,
                    vk::MemoryPropertyFlagBits::eDeviceLocal, m_TextureImage, m_TextureImageMemory);

        TransitionImageLayout(m_TextureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined,
                              vk::ImageLayout::eTransferDstOptimal, m_MipLevels);
        CopyBufferToImage(stagingBuffer, m_TextureImage, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

        GenerateMipmaps(m_TextureImage, width, height, m_MipLevels);

        m_Device.destroyBuffer(stagingBuffer);
        m_Device.freeMemory(stagingBufferMemory);
    }

    void Application::GenerateMipmaps(vk::Image image, int32_t width, int32_t height, uint32_t mipLevels) {
        vk::CommandBuffer commandBuffer = BeginSingleTimeCommands();

        vk::ImageMemoryBarrier barrier{};
        barrier.image = image;
        barrier.srcQueueFamilyIndex = vk::QueueFamilyIgnored;
        barrier.dstQueueFamilyIndex = vk::QueueFamilyIgnored;
        barrier.subresourceRange = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
        };

        int32_t mipWidth = width;
        int32_t mipHeight = height;

        for (uint32_t i = 1; i < mipLevels; i++) {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
            barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

            commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer,
                                          vk::DependencyFlagBits::eByRegion,
                                          0, nullptr,
                                          0, nullptr,
                                          1, &barrier);

            vk::ImageBlit blit{};
            blit.srcOffsets = {{{{}, {static_cast<int32_t>(mipWidth), static_cast<int32_t>(mipHeight), 1}}}};
            blit.srcSubresource = {
                    .aspectMask = vk::ImageAspectFlagBits::eColor,
                    .mipLevel = i - 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1
            };
            blit.dstOffsets = {{{{}, {mipWidth / 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1}}}};
            blit.dstSubresource = {
                    .aspectMask = vk::ImageAspectFlagBits::eColor,
                    .mipLevel = i,
                    .baseArrayLayer = 0,
                    .layerCount = 1
            };

            commandBuffer.blitImage(image, vk::ImageLayout::eTransferSrcOptimal,
                                    image, vk::ImageLayout::eTransferDstOptimal,
                                    1, &blit, vk::Filter::eLinear);

            barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
            barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

            commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                          vk::PipelineStageFlagBits::eFragmentShader,
                                          vk::DependencyFlagBits::eByRegion,
                                          0, nullptr,
                                          0, nullptr,
                                          1, &barrier);

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
        barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

        commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                      vk::PipelineStageFlagBits::eFragmentShader,
                                      vk::DependencyFlagBits::eByRegion,
                                      0, nullptr,
                                      0, nullptr,
                                      1, &barrier);

        EndSingleTimeCommands(commandBuffer);
    }

    vk::ImageView Application::CreateImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags,
                                               uint32_t mipLevels) {
        vk::ImageViewCreateInfo viewInfo{
                .image = image,
                .viewType = vk::ImageViewType::e2D,
                .format = format,
                .subresourceRange {
                        .aspectMask = aspectFlags,
                        .baseMipLevel = 0,
                        .levelCount = mipLevels,
                        .baseArrayLayer = 0,
                        .layerCount = 1
                }
        };

        vk::ImageView imageView;
        if (m_Device.createImageView(&viewInfo, nullptr, &imageView) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create image view");

        return imageView;
    }

    void Application::CreateTextureImageView() {
        m_TextureImageView = CreateImageView(m_TextureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor,
                                             m_MipLevels);
    }

    void Application::CreateTextureSampler() {
        vk::PhysicalDeviceProperties properties = m_PhysicalDevice.getProperties();

        vk::SamplerCreateInfo samplerInfo{
                .magFilter = vk::Filter::eLinear,
                .minFilter = vk::Filter::eLinear,
                .mipmapMode = vk::SamplerMipmapMode::eLinear,
                .addressModeU = vk::SamplerAddressMode::eRepeat,
                .addressModeV = vk::SamplerAddressMode::eRepeat,
                .addressModeW = vk::SamplerAddressMode::eRepeat,
                .mipLodBias = 0.0f,
                .anisotropyEnable = vk::True,
                .maxAnisotropy = properties.limits.maxSamplerAnisotropy,
                .compareEnable = vk::False,
                .compareOp = vk::CompareOp::eAlways,
                .minLod = 0.0f,
                .maxLod = static_cast<float>(m_MipLevels),
                .borderColor = vk::BorderColor::eIntOpaqueBlack,
                .unnormalizedCoordinates = vk::False,
        };

        if (m_Device.createSampler(&samplerInfo, nullptr, &m_TextureSampler) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create texture sampler");
    }

    void Application::TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout,
                                            vk::ImageLayout newLayout, uint32_t mipLevels) {
        vk::CommandBuffer commandBuffer = BeginSingleTimeCommands();

        vk::ImageMemoryBarrier barrier{
                .oldLayout = oldLayout,
                .newLayout = newLayout,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image = image,
                .subresourceRange {
                        .baseMipLevel = 0,
                        .levelCount = mipLevels,
                        .baseArrayLayer = 0,
                        .layerCount = 1
                },
        };

        vk::PipelineStageFlags sourceStage;
        vk::PipelineStageFlags destinationStage;

        if (newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
            barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
        } else {
            barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        }

        if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
            barrier.srcAccessMask = vk::AccessFlagBits::eNone;
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

            sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
            destinationStage = vk::PipelineStageFlagBits::eTransfer;
        } else if (oldLayout == vk::ImageLayout::eTransferDstOptimal &&
                   newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

            sourceStage = vk::PipelineStageFlagBits::eTransfer;
            destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
        } else if (oldLayout == vk::ImageLayout::eUndefined &&
                   newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
            barrier.srcAccessMask = vk::AccessFlagBits::eNone;
            barrier.dstAccessMask =
                    vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

            sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
            destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
        } else {
            throw std::invalid_argument("Unsupported layout transition!");
        }

        commandBuffer.pipelineBarrier(
                sourceStage, destinationStage,
                vk::DependencyFlagBits::eByRegion,
                0, nullptr,
                0, nullptr,
                1, &barrier
        );

        EndSingleTimeCommands(commandBuffer);
    }

    uint32_t Application::FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
        vk::PhysicalDeviceMemoryProperties memoryProperties = m_PhysicalDevice.getMemoryProperties();

        //TODO Slowly understanding that we are using bitwise to check for example 1101 and 1001 then that will bet set if I am correct, still no idea
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
            if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }

        throw std::runtime_error("Failed to find suitable memory type");
    }

    void Application::CreateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
                                   vk::Buffer &buffer, vk::DeviceMemory &bufferMemory) {
        vk::BufferCreateInfo bufferInfo{
                .size = size,
                .usage = usage,
                .sharingMode = vk::SharingMode::eExclusive
        };

        if (m_Device.createBuffer(&bufferInfo, nullptr, &buffer) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create buffer");

        vk::MemoryRequirements memoryRequirements = m_Device.getBufferMemoryRequirements(buffer);

        vk::MemoryAllocateInfo allocateInfo{
                .allocationSize = memoryRequirements.size,
                .memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties)
        };

        if (m_Device.allocateMemory(&allocateInfo, nullptr, &bufferMemory) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to allocate buffer memory");

        m_Device.bindBufferMemory(buffer, bufferMemory, 0);
    }

    void Application::CopyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size) {
        vk::CommandBuffer commandBuffer = BeginSingleTimeCommands();

        vk::BufferCopy copyRegion{
                .size = size
        };
        commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);

        EndSingleTimeCommands(commandBuffer);
    }

    void Application::CreateVertexBuffer() {
        vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        vk::Buffer stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;
        CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                     stagingBuffer, stagingBufferMemory);

        void *data = m_Device.mapMemory(stagingBufferMemory, 0, bufferSize);
        memcpy(data, vertices.data(), (size_t) bufferSize);
        m_Device.unmapMemory(stagingBufferMemory);

        CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                     vk::MemoryPropertyFlagBits::eDeviceLocal, m_VertexBuffer, m_VertexBufferMemory);

        CopyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);
        m_Device.destroyBuffer(stagingBuffer);
        m_Device.freeMemory(stagingBufferMemory);
    }

    void Application::CreateIndexBuffer() {
        vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        vk::Buffer stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;
        CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                     stagingBuffer, stagingBufferMemory);

        void *data = m_Device.mapMemory(stagingBufferMemory, 0, bufferSize);
        memcpy(data, indices.data(), (size_t) bufferSize);
        m_Device.unmapMemory(stagingBufferMemory);

        CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
                     vk::MemoryPropertyFlagBits::eDeviceLocal, m_IndexBuffer, m_IndexBufferMemory);

        CopyBuffer(stagingBuffer, m_IndexBuffer, bufferSize);
        m_Device.destroyBuffer(stagingBuffer);
        m_Device.freeMemory(stagingBufferMemory);
    }

    void Application::CreateUniformBuffers() {
        vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

        m_UniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        m_UniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
        m_UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer,
                         vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                         m_UniformBuffers[i], m_UniformBuffersMemory[i]);

            m_UniformBuffersMapped[i] = m_Device.mapMemory(m_UniformBuffersMemory[i], 0, bufferSize);
        }
    }

    void Application::CreateDescriptorPool() {
        std::array<vk::DescriptorPoolSize, 2> poolSizes{
                vk::DescriptorPoolSize{
                        .type = vk::DescriptorType::eUniformBuffer,
                        .descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)
                },
                vk::DescriptorPoolSize{
                        .type = vk::DescriptorType::eCombinedImageSampler,
                        .descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)
                },
        };

        vk::DescriptorPoolCreateInfo poolInfo{
                .maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
                .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
                .pPoolSizes = poolSizes.data()
        };

        if (m_Device.createDescriptorPool(&poolInfo, nullptr, &m_DescriptorPool) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create descriptor pool");
    }

    void Application::CreateDescriptorSets() {
        std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_DescriptorSetLayout);

        vk::DescriptorSetAllocateInfo allocateInfo{
                .descriptorPool = m_DescriptorPool,
                .descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
                .pSetLayouts = layouts.data()
        };

        m_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        if (m_Device.allocateDescriptorSets(&allocateInfo, m_DescriptorSets.data()) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to allocate descriptor sets");

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vk::DescriptorBufferInfo bufferInfo{
                    .buffer = m_UniformBuffers[i],
                    .offset = 0,
                    .range = sizeof(UniformBufferObject)
            };

            vk::DescriptorImageInfo imageInfo{
                    .sampler = m_TextureSampler,
                    .imageView = m_TextureImageView,
                    .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
            };

            std::array<vk::WriteDescriptorSet, 2> descriptorWrites{
                    vk::WriteDescriptorSet{
                            .dstSet = m_DescriptorSets[i],
                            .dstBinding = 0,
                            .dstArrayElement = 0,
                            .descriptorCount = 1,
                            .descriptorType = vk::DescriptorType::eUniformBuffer,
                            .pBufferInfo = &bufferInfo
                    },
                    vk::WriteDescriptorSet{
                            .dstSet = m_DescriptorSets[i],
                            .dstBinding = 1,
                            .dstArrayElement = 0,
                            .descriptorCount = 1,
                            .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                            .pImageInfo = &imageInfo
                    },
            };


            m_Device.updateDescriptorSets(static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0,
                                          nullptr);
        }
    }

    void Application::CreateCommandBuffers() {
        m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        vk::CommandBufferAllocateInfo allocateInfo{
                .commandPool = m_CommandPool,
                .level = vk::CommandBufferLevel::ePrimary,
                .commandBufferCount = (uint32_t) m_CommandBuffers.size()
        };

        if (m_Device.allocateCommandBuffers(&allocateInfo, m_CommandBuffers.data()) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to allocate command buffers");
    }

    void Application::CreateSyncObjects() {
        m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        vk::SemaphoreCreateInfo semaphoreInfo{};
        vk::FenceCreateInfo fenceInfo{
                .flags = vk::FenceCreateFlagBits::eSignaled
        };

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (m_Device.createSemaphore(&semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) !=
                vk::Result::eSuccess ||
                m_Device.createSemaphore(&semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) !=
                vk::Result::eSuccess ||
                m_Device.createFence(&fenceInfo, nullptr, &m_InFlightFences[i]) != vk::Result::eSuccess)
                throw std::runtime_error("Failed to create Semaphores!");
        }
    }

    void Application::RecordCommandBuffer(vk::CommandBuffer commandBuffer, uint32_t imageIndex) {
        vk::CommandBufferBeginInfo beginInfo{};

        if (commandBuffer.begin(&beginInfo) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to begin recording command buffer");

        std::array<vk::ClearValue, 2> clearValues{
                m_ClearColor,
                vk::ClearValue{
                        .depthStencil {
                                1.0f, 0
                        }
                }
        };

        vk::RenderPassBeginInfo renderPassInfo{
                .renderPass = m_RenderPass,
                .framebuffer = m_SwapchainFramebuffers[imageIndex],
                .renderArea {
                        .offset = {0, 0},
                        .extent = m_SwapchainExtent
                },
                .clearValueCount = static_cast<uint32_t>(clearValues.size()),
                .pClearValues = clearValues.data()
        };

        commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

        if (m_WireframeEnabled) {
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_WireframePipeline);
        } else {
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_GraphicsPipeline);
        }

        vk::Viewport viewport{
                .x = 0.0f,
                .y = (float) m_SwapchainExtent.height,
                .width = (float) m_SwapchainExtent.width,
                .height = -(float) m_SwapchainExtent.height,
                .minDepth = 0.0f,
                .maxDepth = 1.0f
        };

        vk::Rect2D scissor{
                .offset = {0, 0},
                .extent = m_SwapchainExtent
        };

        commandBuffer.setViewport(0, 1, &viewport);
        commandBuffer.setScissor(0, 1, &scissor);

        vk::Buffer vertexBuffers[] = {m_VertexBuffer};
        vk::DeviceSize offsets[] = {0};
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
        commandBuffer.bindIndexBuffer(m_IndexBuffer, 0, vk::IndexType::eUint32);

        commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_PipelineLayout, 0, 1,
                                         &m_DescriptorSets[m_CurrentFrame], 0, nullptr);
        commandBuffer.drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

        commandBuffer.endRenderPass();
        commandBuffer.end();
    }

    void Application::DrawFrame() {
        m_Device.waitForFences(1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        vk::Result result = m_Device.acquireNextImageKHR(m_Swapchain, UINT64_MAX,
                                                         m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE,
                                                         &imageIndex);

        if (result == vk::Result::eErrorOutOfDateKHR) {
            RecreateSwapchain();
            return;
        } else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
            throw std::runtime_error("Failed to acquire swap chain image");

        UpdateUniformBuffer(m_CurrentFrame);

        m_Device.resetFences(1, &m_InFlightFences[m_CurrentFrame]);

        m_CommandBuffers[m_CurrentFrame].reset();
        RecordCommandBuffer(m_CommandBuffers[m_CurrentFrame], imageIndex);

        vk::Semaphore waitSemaphores[] = {m_ImageAvailableSemaphores[m_CurrentFrame]};
        vk::Semaphore signalSemaphores[] = {m_RenderFinishedSemaphores[m_CurrentFrame]};
        vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        vk::SubmitInfo submitInfo{
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = waitSemaphores,
                .pWaitDstStageMask = waitStages,
                .commandBufferCount = 1,
                .pCommandBuffers = &m_CommandBuffers[m_CurrentFrame],
                .signalSemaphoreCount = 1,
                .pSignalSemaphores = signalSemaphores,
        };

        if (m_GraphicsQueue.submit(1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to submit draw command buffer");

        vk::SwapchainKHR swapchains[] = {m_Swapchain};

        vk::PresentInfoKHR presentInfo{
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = signalSemaphores,
                .swapchainCount = 1,
                .pSwapchains = swapchains,
                .pImageIndices = &imageIndex
        };

        result = m_GraphicsQueue.presentKHR(&presentInfo);
        if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || m_FramebufferResized) {
            m_FramebufferResized = false;
            RecreateSwapchain();
        } else if (result != vk::Result::eSuccess)
            throw std::runtime_error("Failed to present swap chain image!");

        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }


    void Application::UpdateUniformBuffer(uint32_t currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f))
                          * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
        UniformBufferObject uniformBufferObject{
                .model = model,
                .view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                                    glm::vec3(0.0f, 0.0f, 3.0f) + glm::vec3(0.0f, 0.0f, -3.0f),
                                    glm::vec3(0.0f, 1.0f, 0.0f)),
                .projection = glm::perspective(glm::radians(45.0f),
                                               (float) m_SwapchainExtent.width / (float) m_SwapchainExtent.height, 0.1f,
                                               10.0f)
        };

        uniformBufferObject.normalInverse = glm::inverse(uniformBufferObject.model);

        memcpy(m_UniformBuffersMapped[currentImage], &uniformBufferObject, sizeof(uniformBufferObject));
    }

    void Application::CleanupVulkan() {
        CleanupSwapchain();

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            m_Device.destroyBuffer(m_UniformBuffers[i]);
            m_Device.freeMemory(m_UniformBuffersMemory[i]);
        }

        m_Device.destroyDescriptorPool(m_DescriptorPool);
        m_Device.destroyDescriptorSetLayout(m_DescriptorSetLayout);

        m_Device.destroySampler(m_TextureSampler);
        m_Device.destroyImageView(m_TextureImageView);
        m_Device.destroyImage(m_TextureImage);
        m_Device.freeMemory(m_TextureImageMemory);

        m_Device.destroyBuffer(m_IndexBuffer);
        m_Device.freeMemory(m_IndexBufferMemory);

        m_Device.destroyBuffer(m_VertexBuffer);
        m_Device.freeMemory(m_VertexBufferMemory);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            m_Device.destroySemaphore(m_ImageAvailableSemaphores[i]);
            m_Device.destroySemaphore(m_RenderFinishedSemaphores[i]);
            m_Device.destroyFence(m_InFlightFences[i]);
        }

        m_Device.destroyCommandPool(m_CommandPool);

        m_Device.destroyPipelineCache(m_GraphicsPipelineCache);
        m_Device.destroyPipeline(m_GraphicsPipeline);
        m_Device.destroyPipeline(m_WireframePipeline);
        m_Device.destroyPipelineLayout(m_PipelineLayout);
        m_Device.destroyRenderPass(m_RenderPass);

        m_Device.destroy();
        m_Instance.destroySurfaceKHR(m_Surface);
        m_Instance.destroy();
    }

#pragma endregion VULKAN

} // Haus