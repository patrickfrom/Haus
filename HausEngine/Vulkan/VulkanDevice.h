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

        static std::shared_ptr<VulkanPhysicalDevice> Select();
    private:
        vk::PhysicalDevice m_PhysicalDevice;

        QueueFamilyIndices m_QueueFamilyIndices;

        bool IsDeviceSuitable(vk::PhysicalDevice device);
    };

    class VulkanDevice {

    };

} // HausEngine
