//
// Created by bauhaus on 08-03-24.
//

#include "VulkanDevice.h"
#include <iostream>
#include "VulkanContext.h"

namespace Haus {
    bool IsDeviceSuitable(vk::PhysicalDevice device) {
        vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
        vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();
        return deviceFeatures.geometryShader && deviceFeatures.fillModeNonSolid && deviceFeatures.samplerAnisotropy;
    }

    void VulkanPhysicalDevice::Select() {
        auto instance = VulkanContext::GetInstance();
        std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
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
} // Haus