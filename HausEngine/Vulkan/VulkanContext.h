#pragma once

#include "VulkanDevice.h"
#include <vulkan/vulkan.hpp>
#include <memory>

namespace HausEngine {

    class VulkanContext {
    public:
        VulkanContext();
        ~VulkanContext();

        std::shared_ptr<VulkanDevice>& GetDevice() {
            return m_VulkanDevice;
        }

        static vk::Instance GetInstance() {
            return s_VulkanInstance;
        }

        static std::shared_ptr<VulkanContext> Create();
    private:
        inline static vk::Instance s_VulkanInstance;

        std::shared_ptr<VulkanPhysicalDevice> m_VulkanPhysicalDevice;
        std::shared_ptr<VulkanDevice> m_VulkanDevice;
    };

} // HausEngine