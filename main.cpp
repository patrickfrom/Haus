#include <iostream>

#define VULKAN_HPP_NO_CONSTRUCTORS

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <set>

GLFWwindow *m_Window;

vk::Instance m_Instance;
vk::PhysicalDevice m_PhysicalDevice;
vk::Device m_Device;
vk::SurfaceKHR m_Surface;
vk::Queue m_GraphicsQueue;
vk::Queue m_PresentQueue;

void Run();

void Loop();

void InitWindow();

void InitVulkan();

void Cleanup();

void CreateInstance();

void CreateSurface();

void PickPhysicalDevice();

void CreateLogicalDevice();

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool IsComplete() const {
        return graphicsFamily.has_value() & presentFamily.has_value();
    }
};

int main() {
    try {
        Run();
    } catch (const std::exception &exception) {
        std::cerr << exception.what() << std::endl;
    }

    Cleanup();
    return 0;
}

void Run() {
    InitWindow();
    InitVulkan();
    Loop();
}

void InitWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_Window = glfwCreateWindow(800, 600, "Haus", nullptr, nullptr);
}

void InitVulkan() {
    CreateInstance();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
}

void Loop() {
    while (!glfwWindowShouldClose(m_Window)) {
        glfwPollEvents();
    }
}

void Cleanup() {
    m_Device.destroy();
    m_Instance.destroySurfaceKHR(m_Surface, nullptr);
    m_Instance.destroy();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void CreateInstance() {
    // Here we specify the application info and the Vulkan API Version
    vk::ApplicationInfo applicationInfo{
            .pApplicationName = "Haus",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_3
    };

    // Then we select the extensions we want to use
    std::vector<const char *> extensions = {
            "VK_KHR_surface",
            "VK_KHR_xcb_surface"
    };

    // Here we create the Instance Info, where we have a reference to our Application Info and then enable the extension count and the Enabled Extension Names
    vk::InstanceCreateInfo instanceCreateInfo{
            .pApplicationInfo = &applicationInfo,
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data()
    };

    // Then we create our instance, with the instance create info and check if the instance was created, if not it will fail.
    m_Instance = vk::createInstance(instanceCreateInfo, nullptr);
    if (!m_Instance)
        throw std::runtime_error("Failed to create Instance!");
}

void CreateSurface() {
    // Here we create our surface between GLFW
    auto surfaceKHR = static_cast<VkSurfaceKHR>(m_Surface);
    if (static_cast<vk::Result>(glfwCreateWindowSurface(VkInstance(m_Instance), m_Window, nullptr, &surfaceKHR)) !=
        vk::Result::eSuccess)
        throw std::runtime_error("Failed to create Window Surface");

    m_Surface = surfaceKHR;
}

void PickPhysicalDevice() {
    // Here we get all the Physical Devices and if it's empty then we will throw a No Physical Device Runtime Error
    std::vector<vk::PhysicalDevice> devices = m_Instance.enumeratePhysicalDevices();
    if (devices.empty())
        throw std::runtime_error("No Physical Device");

    // Here I select the first Physical Device
    m_PhysicalDevice = devices[0];
    vk::PhysicalDeviceProperties deviceProperties = m_PhysicalDevice.getProperties();
    std::cout << deviceProperties.deviceName << std::endl;
}

QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device) {
    QueueFamilyIndices indices;
    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
            indices.graphicsFamily = i;

        VkBool32 presentSupport = device.getSurfaceSupportKHR(i, m_Surface);
        if (presentSupport)
            indices.presentFamily = i;

        if (indices.IsComplete())
            break;

        i++;
    }

    return indices;
}

void CreateLogicalDevice() {
    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
            indices.graphicsFamily.value(),
            indices.presentFamily.value()
    };

    const float priority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo queueCreateInfo{
            .queueFamilyIndex = queueFamily,
            .queueCount = 1,
            .pQueuePriorities = &priority,
        };

        queueCreateInfos.push_back(queueCreateInfo);
    }

    std::vector<const char*> enabledExtensions = { "VK_KHR_swapchain" };

    vk::PhysicalDeviceFeatures deviceFeatures{};

    vk::DeviceCreateInfo createInfo{
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size()),
        .ppEnabledExtensionNames = enabledExtensions.data(),
        .pEnabledFeatures = &deviceFeatures
    };

    m_Device = m_PhysicalDevice.createDevice(createInfo, nullptr);
    if (!m_Device)
        throw std::runtime_error("Failed to create Logical Device");

    m_GraphicsQueue = m_Device.getQueue(indices.graphicsFamily.value(), 0);
    m_PresentQueue = m_Device.getQueue(indices.presentFamily.value(), 0);
}