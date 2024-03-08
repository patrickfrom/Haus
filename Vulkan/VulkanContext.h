//
// Created by bauhaus on 08-03-24.
//

#ifndef HAUS_VULKANCONTEXT_H
#define HAUS_VULKANCONTEXT_H

#include <vulkan/vulkan.hpp>
#include "VulkanDevice.h"
#include <memory>

namespace Haus {
    const std::vector<const char *> VALIDATION_LAYERS = {
            "VK_LAYER_KHRONOS_validation"
    };

#ifdef NDEBUG
    const bool ENABLE_VALIDATION_LAYERS = false;
#else
    const bool ENABLE_VALIDATION_LAYERS = true;
#endif

    class VulkanContext {
    public:
        VulkanContext();

        ~VulkanContext();

        static vk::Instance GetInstance() {
            return s_VulkanInstance;
        }

        // Just doing this for now until I create the class for Vulkan Logical Device
        std::shared_ptr<VulkanPhysicalDevice> GetVulkanPhysicalDevice() {
            return m_VulkanPhysicalDevice;
        }

    private:
        inline static vk::Instance s_VulkanInstance;

        std::shared_ptr<VulkanPhysicalDevice> m_VulkanPhysicalDevice;

        void Init();
    };

} // Haus

#endif //HAUS_VULKANCONTEXT_H
