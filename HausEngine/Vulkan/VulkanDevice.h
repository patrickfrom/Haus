#pragma once

#include <vulkan/vulkan.hpp>
#include <memory>

namespace HausEngine {

    class VulkanPhysicalDevice {
    public:
        struct QueueFamilyIndices {
            std::optional<uint32_t> Graphics;

            [[nodiscard]] bool IsComplete() const {
                return Graphics.has_value();
            }
        };
    public:
        VulkanPhysicalDevice();

        vk::PhysicalDevice GetPhysicalDevice() {
            return m_PhysicalDevice;
        }

        QueueFamilyIndices GetQueueFamilyIndices() {
            return m_QueueFamilyIndices;
        }

        static std::shared_ptr<VulkanPhysicalDevice> Select();
    private:
        vk::PhysicalDevice m_PhysicalDevice;

        QueueFamilyIndices m_QueueFamilyIndices;

        bool IsDeviceSuitable(vk::PhysicalDevice device);
    };

    class VulkanDevice {
    public:
        VulkanDevice(const std::shared_ptr<VulkanPhysicalDevice>& physicalDevice, vk::PhysicalDeviceFeatures enabledFeatures);

        void Destroy();

        vk::Queue GetGraphicsQueue() {
            return m_GraphicsQueue;
        }

        [[nodiscard]] const std::shared_ptr<VulkanPhysicalDevice>& GetPhysicalDevice() const {
            return m_PhysicalDevice;
        }

        [[nodiscard]] vk::Device GetDevice() const {
            return m_LogicalDevice;
        }
    private:
        vk::Device m_LogicalDevice;

        std::shared_ptr<VulkanPhysicalDevice> m_PhysicalDevice;
        vk::PhysicalDeviceFeatures m_EnabledFeatures;

        vk::Queue m_GraphicsQueue;
    };

} // HausEngine
