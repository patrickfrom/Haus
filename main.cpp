
#include <iostream>

#define VULKAN_HPP_NO_CONSTRUCTORS

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

GLFWwindow *window;

vk::Instance instance;
vk::PhysicalDevice physicalDevice;
vk::Device device;
vk::SurfaceKHR surface;
vk::Queue graphicsQueue;

void Run();

void Loop();

void InitWindow();

void InitVulkan();

void Cleanup();

void CreateInstance();

void CreateSurface();

void PickPhysicalDevice();

void CreateLogicalDevice();

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

    window = glfwCreateWindow(800, 600, "Haus", nullptr, nullptr);
}

void InitVulkan() {
    CreateInstance();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
}

void Loop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void Cleanup() {
    device.destroy();
    instance.destroy();

    glfwDestroyWindow(window);
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

    // Then we create our instance, with the instance create info and check if the instace was created, if not it will fail.
    instance = vk::createInstance(instanceCreateInfo, nullptr);;
    if (!instance)
        throw std::runtime_error("Failed to create Instance!");
}

void CreateSurface() {
    // Here we create our surface between GLFW
    auto surfaceKHR = static_cast<VkSurfaceKHR>(surface);
    if (static_cast<vk::Result>(glfwCreateWindowSurface(VkInstance(instance), window, nullptr, &surfaceKHR)) !=
        vk::Result::eSuccess)
        throw std::runtime_error("Failed to create Window Surface");
}

void PickPhysicalDevice() {
    // Here we get all the Physical Devices and if it's empty then we will throw a No Physical Device Runtime Error
    std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
    if (devices.empty())
        throw std::runtime_error("No Physical Device");

    // Here I select the first Physical Device
    physicalDevice = devices[0];
    vk::PhysicalDeviceProperties deviceProperties = physicalDevice.getProperties();
    std::cout << deviceProperties.deviceName << std::endl;
}

void CreateLogicalDevice() {
    std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();

    int queueFamilyIndex = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
            break;

        queueFamilyIndex++;
    }

    const float priority = 1.0f;
    vk::DeviceQueueCreateInfo queueCreateInfo{
        .queueFamilyIndex = static_cast<uint32_t>(queueFamilyIndex),
        .queueCount = 1,
        .pQueuePriorities = &priority,
    };

    std::vector<const char*> enabledExtensions = { "VK_KHR_swapchain" };

    vk::PhysicalDeviceFeatures deviceFeatures{};

    vk::DeviceCreateInfo createInfo{
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queueCreateInfo,
        .enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size()),
        .ppEnabledExtensionNames = enabledExtensions.data(),
        .pEnabledFeatures = &deviceFeatures
    };

    device = physicalDevice.createDevice(createInfo, nullptr);
    if (!device)
        throw std::runtime_error("Failed to create Logical Device");

    graphicsQueue = device.getQueue(queueFamilyIndex, 0);
}