#include "Application.h"
#include <iostream>
#include <format>
#include <fstream>

/* Currently using regions, just so it's easier for me to
   see what's going on since I don't want to abstract
   Vulkan or GLFW until I understand them a little better*/
namespace Haus {
#pragma region APPLICATION

    Application::Application(ApplicationSpecification &specification) : m_Specification(specification) {}

    void Application::Run() {
        std::cout << std::format("Starting Application {}", m_Specification.Name) << std::endl;

        InitWindow();
        InitVulkan();
        Loop();
    }

    void Application::Shutdown() {
        std::cout << std::format("Closing Application {}", m_Specification.Name) << std::endl;

        CleanupVulkan();
        CleanupGLFW();
    }

    void Application::Loop() {
        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();
        }
    }

#pragma endregion APPLICATION

#pragma region GLFW

    void Application::InitWindow() {
        if (!glfwInit())
            throw std::runtime_error("Failed to Initialize GLFW");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_Window = glfwCreateWindow(m_Specification.Width, m_Specification.Height, m_Specification.Name.c_str(),
                                    nullptr, nullptr);
        if (!m_Window)
            throw std::runtime_error("Failed to create GLFW Window");
    }

    void Application::CleanupGLFW() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

#pragma endregion GLFW

#pragma region VULKAN

    void Application::InitVulkan() {
        std::cout << "Initializing Vulkan" << std::endl;
        CreateInstance();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapchain();
        CreateImageViews();
        CreateRenderPass();
        CreateGraphicsPipeline();
    }

    void Application::CreateInstance() {
        vk::ApplicationInfo appInfo{
                .pApplicationName = m_Specification.Name.c_str(),
                .applicationVersion = VK_MAKE_VERSION(1, 0, 0),

                .pEngineName = "No Engine",
                .engineVersion = VK_MAKE_VERSION(1, 0, 0),

                .apiVersion = VK_API_VERSION_1_2
        };

        uint32_t glfwExtensionCount = 0;
        const char **extensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        vk::InstanceCreateInfo createInfo{
                .pApplicationInfo = &appInfo,
                .enabledExtensionCount = glfwExtensionCount,
                .ppEnabledExtensionNames = extensions
        };

        m_Instance = vk::createInstance(createInfo);
        if (!m_Instance)
            throw std::runtime_error("Failed to create Instance");
    }

    void Application::CreateSurface() {
        if (glfwCreateWindowSurface(m_Instance, m_Window, nullptr, reinterpret_cast<VkSurfaceKHR *>(&m_Surface)))
            throw std::runtime_error("Failed to create window surface!");
    }

    bool IsDeviceSuitable(vk::PhysicalDevice device) {
        vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
        vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();
        return deviceFeatures.geometryShader;
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
    }

    void Application::CreateLogicalDevice() {
        float priority = 1.0f;
        vk::DeviceQueueCreateInfo queueCreateInfo{
                .queueFamilyIndex = 0,
                .queueCount = 1,
                .pQueuePriorities = &priority,
        };

        vk::PhysicalDeviceFeatures deviceFeatures{};

        std::array<const char *, 1> enabledExtensions = {"VK_KHR_swapchain"};

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
        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        vk::Extent2D extent{
                .width = static_cast<uint32_t>(width),
                .height = static_cast<uint32_t>(height)
        };

        vk::SwapchainCreateInfoKHR createInfo{
                .surface = m_Surface,
                .minImageCount = 4,
                .imageFormat = swapChainSupport.Formats[0].format,
                .imageColorSpace = swapChainSupport.Formats[0].colorSpace,
                .imageExtent = extent,
                .imageArrayLayers = 1,
                .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
                .imageSharingMode = vk::SharingMode::eExclusive,
                .preTransform = swapChainSupport.Capabilities.currentTransform,
                .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
                .presentMode = vk::PresentModeKHR::eFifo,
                .clipped = VK_TRUE,
        };

        if (m_Device.createSwapchainKHR(&createInfo, nullptr, &m_Swapchain) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create swap chain");

        m_SwapchainImages = m_Device.getSwapchainImagesKHR(m_Swapchain);

        m_SwapchainImageFormat = swapChainSupport.Formats[0].format;
        m_SwapchainExtent = extent;
    }

    void Application::CreateImageViews() {
        m_SwapchainImageViews.resize(m_SwapchainImages.size());

        for (size_t i = 0; i < m_SwapchainImages.size(); i++) {
            vk::ImageViewCreateInfo createInfo{
                    .image = m_SwapchainImages[i],
                    .viewType = vk::ImageViewType::e2D,
                    .format = m_SwapchainImageFormat,
                    .components {
                            .r = vk::ComponentSwizzle::eIdentity,
                            .g = vk::ComponentSwizzle::eIdentity,
                            .b = vk::ComponentSwizzle::eIdentity,
                            .a = vk::ComponentSwizzle::eIdentity,
                    },
                    .subresourceRange {
                            .aspectMask = vk::ImageAspectFlagBits::eColor,
                            .baseMipLevel = 0,
                            .levelCount = 1,
                            .baseArrayLayer = 0,
                            .layerCount = 1
                    }
            };

            if (m_Device.createImageView(&createInfo, nullptr, &m_SwapchainImageViews[i]) != vk::Result::eSuccess)
                throw std::runtime_error("Failed to create image views!");
        }
    }

    void Application::CreateRenderPass() {
        vk::AttachmentDescription colorAttachment {
            .format = m_SwapchainImageFormat,
            .samples = vk::SampleCountFlagBits::e1,
            .loadOp = vk::AttachmentLoadOp::eClear,
            .storeOp = vk::AttachmentStoreOp::eStore,
            .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
            .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
            .initialLayout = vk::ImageLayout::eUndefined,
            .finalLayout = vk::ImageLayout::ePresentSrcKHR
        };

        vk::AttachmentReference colorAttachmentReference {
            .attachment = 0,
            .layout = vk::ImageLayout::eColorAttachmentOptimal
        };

        vk::SubpassDescription subpass{
            .pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
            .colorAttachmentCount = 1,
            .pColorAttachments = &colorAttachmentReference,
        };

        // https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/02_Graphics_pipeline_basics/03_Render_passes.html#_render_pass
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

    void Application::CreateGraphicsPipeline() {
        auto vertexShaderCode = ReadFile("shaders/vert.spv");
        auto fragmentShaderCode = ReadFile("shaders/frag.spv");

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

        std::array<vk::DynamicState, 2> dynamicStates = {
                vk::DynamicState::eViewport,
                vk::DynamicState::eScissor
        };

        vk::PipelineDynamicStateCreateInfo dynamicState{
                .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
                .pDynamicStates = dynamicStates.data()
        };

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
                .vertexBindingDescriptionCount = 0,
                .vertexAttributeDescriptionCount = 0,
        };

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
                .topology = vk::PrimitiveTopology::eTriangleList,
                .primitiveRestartEnable = VK_FALSE
        };

        vk::Viewport viewport{
                .x = 0.0f,
                .y = 0.0f,
                .width = (float) m_SwapchainExtent.width,
                .height = (float) m_SwapchainExtent.height,
                .minDepth = 0.0f,
                .maxDepth = 1.0f,
        };

        vk::Rect2D scissor{
                .offset = {0, 0},
                .extent = m_SwapchainExtent
        };

        vk::PipelineViewportStateCreateInfo viewportState{
                .viewportCount = 1,
                .pViewports = &viewport,
                .scissorCount = 1,
                .pScissors = &scissor
        };

        vk::PipelineRasterizationStateCreateInfo rasterizer{
                .depthClampEnable = VK_FALSE,
                .rasterizerDiscardEnable = VK_FALSE,
                .polygonMode = vk::PolygonMode::eFill,
                .cullMode = vk::CullModeFlagBits::eBack,
                .frontFace = vk::FrontFace::eClockwise,
                .depthBiasEnable = VK_FALSE,
                .lineWidth = 1.0f,
        };

        vk::PipelineMultisampleStateCreateInfo multisampling{
                .rasterizationSamples = vk::SampleCountFlagBits::e1,
                .sampleShadingEnable = VK_FALSE,
        };

        vk::PipelineColorBlendAttachmentState colorBlendAttachment{
                .blendEnable = VK_FALSE,
                .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG
                                  | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
        };

        vk::PipelineColorBlendStateCreateInfo colorBlending{
                .logicOpEnable = VK_FALSE,
                .logicOp = vk::LogicOp::eCopy,
                .attachmentCount = 1,
                .pAttachments = &colorBlendAttachment
        };

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};

        if (m_Device.createPipelineLayout(&pipelineLayoutInfo, nullptr, &m_PipelineLayout) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create pipeline layout!");

        m_Device.destroyShaderModule(vertexShaderModule);
        m_Device.destroyShaderModule(fragmentShaderModule);
    }

    void Application::CleanupVulkan() {
        m_Device.destroyPipelineLayout(m_PipelineLayout);

        for (auto imageView: m_SwapchainImageViews)
            m_Device.destroyImageView(imageView);

        m_Device.destroySwapchainKHR(m_Swapchain);
        m_Device.destroy();
        m_Instance.destroySurfaceKHR(m_Surface);
        m_Instance.destroy();
    }

#pragma endregion VULKAN

} // Haus