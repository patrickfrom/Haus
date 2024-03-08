//
// Created by bauhaus on 08-03-24.
//

#ifndef HAUS_VULKANDEVICE_H
#define HAUS_VULKANDEVICE_H

#include <vulkan/vulkan.hpp>

namespace Haus {

    class VulkanPhysicalDevice {
    public:
        VulkanPhysicalDevice() = default;

        ~VulkanPhysicalDevice() = default;

        void Select();

        vk::PhysicalDevice GetPhysicalDevice() {
            return m_PhysicalDevice;
        }

    private:
        vk::PhysicalDevice m_PhysicalDevice;

        friend class VulkanDevice;
    };

    class VulkanDevice {

    };

} // Haus

#endif //HAUS_VULKANDEVICE_H
