#include "VulkanDevice.h"
#include <iostream>
#include "VulkanContext.h"

namespace HausEngine {
    // Physical Device
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

    bool VulkanPhysicalDevice::IsDeviceSuitable(vk::PhysicalDevice device) {
        vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
        vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();

        int i = 0;
        for (auto queueFamily: device.getQueueFamilyProperties()) {
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                m_QueueFamilyIndices.Graphics = i;
            }

            if (m_QueueFamilyIndices.IsComplete())
                break;

            i++;
        }

        return m_QueueFamilyIndices.IsComplete();
    }

    std::shared_ptr<VulkanPhysicalDevice> VulkanPhysicalDevice::Select() {
        return std::make_shared<VulkanPhysicalDevice>();
    }

    // Device
    VulkanDevice::VulkanDevice(const std::shared_ptr<VulkanPhysicalDevice> &physicalDevice,
                               vk::PhysicalDeviceFeatures enabledFeatures) : m_PhysicalDevice(physicalDevice),
                                                                             m_EnabledFeatures(enabledFeatures) {
        auto vkPhysicalDevice = m_PhysicalDevice->GetPhysicalDevice();

        float priority = 1.0f;
        vk::DeviceQueueCreateInfo queueInfo{};
        queueInfo.queueFamilyIndex = 0;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &priority;

        std::vector<const char *> enabledExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        vk::DeviceCreateInfo createInfo{};
        createInfo.queueCreateInfoCount = 1;
        createInfo.pQueueCreateInfos = &queueInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
        createInfo.ppEnabledExtensionNames = enabledExtensions.data();
        createInfo.pEnabledFeatures = &enabledFeatures;

        if (vkPhysicalDevice.createDevice(&createInfo, nullptr, &m_LogicalDevice) != vk::Result::eSuccess)
            throw std::runtime_error("Failed to create Vulkan Logical Device");

        m_GraphicsQueue = m_LogicalDevice.getQueue(0, m_PhysicalDevice->GetQueueFamilyIndices().Graphics.value());
    }

    void VulkanDevice::Destroy() {
        m_LogicalDevice.waitIdle();
        m_LogicalDevice.destroy();
    }

    std::shared_ptr<VulkanDevice> VulkanDevice::Create(const std::shared_ptr<VulkanPhysicalDevice> &physicalDevice,
                                                       vk::PhysicalDeviceFeatures enabledFeatures) {
        return std::make_shared<VulkanDevice>(physicalDevice, enabledFeatures);
    }
} // HausEngine
