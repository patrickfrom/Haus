#include "VulkanDevice.h"
#include <iostream>
#include "VulkanContext.h"

namespace HausEngine {
    bool VulkanPhysicalDevice::IsDeviceSuitable(vk::PhysicalDevice device) {
        vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
        vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();

        int i = 0;
        for (auto queueFamily : device.getQueueFamilyProperties()) {
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                m_QueueFamilyIndices.Graphics = i;
            }

            if (m_QueueFamilyIndices.IsComplete())
                break;

            i++;
        }

        return m_QueueFamilyIndices.IsComplete();
    }

    VulkanPhysicalDevice::VulkanPhysicalDevice() {
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

    std::shared_ptr<VulkanPhysicalDevice> VulkanPhysicalDevice::Select() {
        return std::make_shared<VulkanPhysicalDevice>();
    }
} // HausEngine