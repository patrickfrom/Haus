#include "Application.h"
#include <iostream>
#include <format>

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
        SwapChainSupportDetails details {
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

    void Application::CleanupVulkan() {
        m_Device.destroySwapchainKHR(m_Swapchain);
        m_Device.destroy();
        m_Instance.destroySurfaceKHR(m_Surface);
        m_Instance.destroy();
    }

#pragma endregion VULKAN

} // Haus