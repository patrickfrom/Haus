#pragma once

#include "Ref.h"
#include "VulkanDevice.h"
#include <vulkan/vulkan.hpp>
#include <memory>

namespace HausEngine {

    class VulkanContext {
    public:
        VulkanContext();
        ~VulkanContext();

        Ref<VulkanDevice>& GetDevice() {
            return m_VulkanDevice;
        }

        static vk::Instance GetInstance() {
            return s_VulkanInstance;
        }
    private:
        inline static vk::Instance s_VulkanInstance;

        std::shared_ptr<VulkanPhysicalDevice> m_VulkanPhysicalDevice;
        Ref<VulkanDevice> m_VulkanDevice;
    };

} // HausEngine