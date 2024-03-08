#pragma once

#include <vulkan/vulkan.hpp>
#include <memory>

namespace HausEngine {

    class VulkanPhysicalDevice {
    public:
        VulkanPhysicalDevice();

        static std::shared_ptr<VulkanPhysicalDevice> Select();
    private:
        vk::PhysicalDevice m_PhysicalDevice;
    };

    class VulkanDevice {

    };

} // HausEngine
