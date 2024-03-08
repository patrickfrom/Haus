#include "VulkanContext.h"
#include <iostream>
#include <GLFW/glfw3.h>

namespace HausEngine {
    VulkanContext::VulkanContext() {
        vk::ApplicationInfo appInfo{};
        appInfo.pApplicationName = "Haus";
        appInfo.pEngineName = "HausEngine";
        appInfo.apiVersion = VK_API_VERSION_1_3;

        uint32_t extensionCount;
        const char** enabledExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
        vk::InstanceCreateInfo createInfo{};
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = extensionCount;
        createInfo.ppEnabledExtensionNames = enabledExtensions;

        if (vk::createInstance(&createInfo, nullptr, &s_VulkanInstance) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create Vulkan Instance");


        m_VulkanPhysicalDevice = VulkanPhysicalDevice::Select();

        vk::PhysicalDeviceFeatures features{};
        m_VulkanDevice = VulkanDevice::Create(m_VulkanPhysicalDevice, features);
    }

    VulkanContext::~VulkanContext() {
        vkDestroyInstance(s_VulkanInstance, nullptr);
        s_VulkanInstance = nullptr;
    }

    std::shared_ptr<VulkanContext> VulkanContext::Create() {
        return std::make_shared<VulkanContext>();
    }
} // HausEngine