#pragma once

#include "VulkanDevice.h"
#include <vulkan/vulkan.hpp>
#include <memory>

namespace HausEngine {

    class VulkanContext {
    public:
        VulkanContext();
        ~VulkanContext();

        static vk::Instance GetInstance() {
            return s_VulkanInstance;
        }

        static std::shared_ptr<VulkanContext> Create();
    private:
        inline static vk::Instance s_VulkanInstance;

        std::shared_ptr<VulkanPhysicalDevice> m_VulkanPhysicalDevice;
    };

} // HausEngine