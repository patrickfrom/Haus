#include "VulkanContext.h"
#include <iostream>
#include <vector>

namespace HausEngine {
    VulkanContext::VulkanContext() {
        vk::ApplicationInfo appInfo{};
        appInfo.pApplicationName = "Haus";
        appInfo.pEngineName = "HausEngine";
        appInfo.apiVersion = VK_API_VERSION_1_3;

        vk::InstanceCreateInfo createInfo{};
        createInfo.pApplicationInfo = &appInfo;

        if (vk::createInstance(&createInfo, nullptr, &s_VulkanInstance) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create Vulkan Instance");


        // Pick Physical Device & Queues
        m_VulkanPhysicalDevice = VulkanPhysicalDevice::Select();

        // Create Logical Device & Queues
    }

    VulkanContext::~VulkanContext() {
        vkDestroyInstance(s_VulkanInstance, nullptr);
        s_VulkanInstance = nullptr;
    }

    std::shared_ptr<VulkanContext> VulkanContext::Create() {
        return std::make_shared<VulkanContext>();
    }
} // HausEngine