//
// Created by bauhaus on 08-03-24.
//

#include "VulkanContext.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Haus {
    bool CheckValidationLayerSupport() {
        auto availableLayers = vk::enumerateInstanceLayerProperties();
        for (const char* layerName : VALIDATION_LAYERS) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
                return false;
        }

        return true;
    }

    VulkanContext::VulkanContext() {
        Init();
    };

    VulkanContext::~VulkanContext() {
        std::cout << "Destructed Vulkan Context" << "\n";
        delete m_VulkanPhysicalDevice;

        s_VulkanInstance.destroy();
        s_VulkanInstance = nullptr;

    }

    void VulkanContext::Init() {
        if (ENABLE_VALIDATION_LAYERS && !CheckValidationLayerSupport) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        vk::ApplicationInfo appInfo{};
        appInfo.pApplicationName = "Haus";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

        appInfo.apiVersion = VK_API_VERSION_1_3;

        uint32_t glfwExtensionCount = 0;
        const char **extensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        vk::InstanceCreateInfo createInfo{};
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = extensions;

        if (ENABLE_VALIDATION_LAYERS) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
            createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        vk::Result result = vk::createInstance(&createInfo, nullptr, &s_VulkanInstance);
        if (result != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create Instance");

        m_VulkanPhysicalDevice = new VulkanPhysicalDevice();
        m_VulkanPhysicalDevice->Select();
    }
} // Haus