#include "Application.h"
#include <iostream>
#include <format>
#include <fstream>
#include <glm/glm.hpp>

/* Currently using regions, just so it's easier for me to
   see what's going on since I don't want to abstract
   Vulkan or GLFW until I understand them a little better*/
namespace Haus {
    struct Vertex {
        glm::vec2 Position;
        glm::vec3 Color;

        static vk::VertexInputBindingDescription GetBindingDescription() {
            vk::VertexInputBindingDescription bindingDescription{
                    .binding = 0,
                    .stride = sizeof(Vertex),
                    .inputRate = vk::VertexInputRate::eVertex
            };

            return bindingDescription;
        }

        static std::array<vk::VertexInputAttributeDescription, 2> GetAttributeDescriptions() {
            std::array<vk::VertexInputAttributeDescription, 2> attributeDescription{
                    vk::VertexInputAttributeDescription{
                            .location = 0,
                            .binding = 0,
                            .format = vk::Format::eR32G32Sfloat,
                            .offset = offsetof(Vertex, Position)
                    },
                    vk::VertexInputAttributeDescription{
                            .location = 1,
                            .binding = 0,
                            .format = vk::Format::eR32G32B32Sfloat,
                            .offset = offsetof(Vertex, Color)
                    }
            };

            return attributeDescription;
        }
    };

    // TODO: Later this will use Indices
    const std::vector<Vertex> vertices = {
            {{0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f},  {1.0f, 0.32f, 0.32f}},
            {{-0.5f,  -0.5f},  {1.0f, 0.29f, 0.14f}},

            {{-0.5f,  -0.5f},  {1.0f, 0.29f, 0.14f}},
            {{-0.5f, 0.5f},  {1.0f, 0.32f, 0.32f}},
            {{0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
    };

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
        float time = 0;
        while (!glfwWindowShouldClose(m_Window)) {
            time += 0.001;

            glfwPollEvents();
            SetClearColor({glm::sin(time), glm::cos(time * 0.25), glm::cos(time), 1.0f});
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
    }

    void Application::FramebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

        app->m_FramebufferResized = true;
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
        CreateFramebuffers();
        CreateCommandPool();
        CreateVertexBuffer();
        CreateCommandBuffers();
        CreateSyncObjects();
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

        m_Instance = vk::createInstance(createInfo);
        if (!m_Instance)
            throw std::runtime_error("Failed to create Instance");
    }

    void Application::CreateSurface() {
        if (glfwCreateWindowSurface(m_Instance, m_Window, nullptr, reinterpret_cast<VkSurfaceKHR *>(&m_Surface)))
            throw std::runtime_error("Failed to create window surface!");
    }

    // TODO - Do Later perhaps?
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

        vk::PhysicalDeviceProperties deviceProperties = m_PhysicalDevice.getProperties();
        std::cout << deviceProperties.deviceName << std::endl;
        std::cout << to_string(deviceProperties.deviceType) << std::endl;
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

        uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;
        if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
            imageCount = swapChainSupport.Capabilities.maxImageCount;

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
        CreateFramebuffers();
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

        vk::SubpassDescription subpass{
                .pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
                .colorAttachmentCount = 1,
                .pColorAttachments = &colorAttachmentReference,
        };

        vk::SubpassDependency dependency{
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
                .dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
                .srcAccessMask = vk::AccessFlagBits::eNone,
                .dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite
        };

        vk::RenderPassCreateInfo renderPassInfo{
                .attachmentCount = 1,
                .pAttachments = &colorAttachment,
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

        std::array<vk::DynamicState, 2> dynamicStates = {
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

        vk::GraphicsPipelineCreateInfo pipelineInfo{
                .stageCount = static_cast<uint32_t>(shaderStages.size()),
                .pStages = shaderStages.data(),
                .pVertexInputState = &vertexInputInfo,
                .pInputAssemblyState = &inputAssembly,
                .pViewportState = &viewportState,
                .pRasterizationState = &rasterizer,
                .pMultisampleState = &multisampling,
                .pColorBlendState = &colorBlending,
                .pDynamicState = &dynamicState,
                .layout = m_PipelineLayout,
                .renderPass = m_RenderPass,
                .subpass = 0,

                // Vulkan allows you to create a new graphics pipeline by deriving from an existing pipeline.
                // The idea of pipeline derivatives is that it is less expensive to set up pipelines when they have much functionality
                // in common with an existing pipeline and switching between pipelines from the same parent can also be done quicker.
                .basePipelineHandle = VK_NULL_HANDLE,
                .basePipelineIndex = -1
        };

        if (m_Device.createGraphicsPipelines(VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) !=
            vk::Result::eSuccess)
            throw std::runtime_error("Failed to create Graphics Pipeline");

        m_Device.destroyShaderModule(vertexShaderModule);
        m_Device.destroyShaderModule(fragmentShaderModule);
    }

    void Application::CreateFramebuffers() {
        m_SwapchainFramebuffers.resize(m_SwapchainImageViews.size());

        for (size_t i = 0; i < m_SwapchainImageViews.size(); i++) {
            vk::ImageView attachments[] = {
                    m_SwapchainImageViews[i]
            };

            vk::FramebufferCreateInfo framebufferInfo{
                    .renderPass = m_RenderPass,
                    .attachmentCount = 1,
                    .pAttachments = attachments,
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

    uint32_t Application::FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
        vk::PhysicalDeviceMemoryProperties memoryProperties = m_PhysicalDevice.getMemoryProperties();

        //TODO No idea how this works yet tbh, my brain can't wrap around it so take a deeper look into this later on
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
            std::cout << to_string(memoryProperties.memoryTypes[i].propertyFlags) << std::endl;
            if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }

        throw std::runtime_error("Failed to find suitable memory type");
    }

    void Application::CreateVertexBuffer() {
        vk::BufferCreateInfo bufferInfo{
                .size = sizeof(vertices[0]) * vertices.size(),
                .usage = vk::BufferUsageFlagBits::eVertexBuffer,
                .sharingMode = vk::SharingMode::eExclusive
        };

        if (m_Device.createBuffer(&bufferInfo, nullptr, &m_VertexBuffer) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create vertex buffer");

        vk::MemoryRequirements memoryRequirements = m_Device.getBufferMemoryRequirements(m_VertexBuffer);

        vk::MemoryAllocateInfo allocateInfo{
                .allocationSize = memoryRequirements.size,
                .memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits,
                                                  vk::MemoryPropertyFlagBits::eHostVisible |
                                                  vk::MemoryPropertyFlagBits::eHostCoherent),
        };

        if (m_Device.allocateMemory(&allocateInfo, nullptr, &m_VertexBufferMemory) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to allocate vertex buffer memory!");

        m_Device.bindBufferMemory(m_VertexBuffer, m_VertexBufferMemory, 0);

        void* data = m_Device.mapMemory(m_VertexBufferMemory, 0, bufferInfo.size);
        memcpy(data, vertices.data(), (size_t) bufferInfo.size);
        m_Device.unmapMemory(m_VertexBufferMemory);
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

        vk::RenderPassBeginInfo renderPassInfo{
                .renderPass = m_RenderPass,
                .framebuffer = m_SwapchainFramebuffers[imageIndex],
                .renderArea {
                        .offset = {0, 0},
                        .extent = m_SwapchainExtent
                },
                .clearValueCount = 1,
                .pClearValues = &m_ClearColor
        };

        commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_GraphicsPipeline);

        vk::Viewport viewport{
                .x = 0.0f,
                .y = (float) m_SwapchainExtent.height,
                .width = (float) m_SwapchainExtent.width,
                .height = -(float) m_SwapchainExtent.height,
                .minDepth = 0.0f,
                .maxDepth = 1.0f,
        };

        vk::Rect2D scissor{
                .offset = {0, 0},
                .extent = m_SwapchainExtent
        };

        commandBuffer.setViewport(0, 1, &viewport);
        commandBuffer.setScissor(0, 1, &scissor);

        vk::Buffer vertexBuffers[] = {m_VertexBuffer};
        vk::DeviceSize  offsets[] = {0};
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

        commandBuffer.draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0);

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

    void Application::CleanupVulkan() {
        CleanupSwapchain();

        m_Device.destroyBuffer(m_VertexBuffer);
        m_Device.freeMemory(m_VertexBufferMemory);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            m_Device.destroySemaphore(m_ImageAvailableSemaphores[i]);
            m_Device.destroySemaphore(m_RenderFinishedSemaphores[i]);
            m_Device.destroyFence(m_InFlightFences[i]);
        }

        m_Device.destroyCommandPool(m_CommandPool);

        m_Device.destroyPipeline(m_GraphicsPipeline);
        m_Device.destroyPipelineLayout(m_PipelineLayout);
        m_Device.destroyRenderPass(m_RenderPass);

        m_Device.destroy();
        m_Instance.destroySurfaceKHR(m_Surface);
        m_Instance.destroy();
    }

#pragma endregion VULKAN

} // Haus