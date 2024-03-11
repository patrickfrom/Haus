#pragma once

#include "VulkanContext.h"
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <vector>

namespace HausEngine {
    struct SwapchainSupportDetails {
        vk::SurfaceCapabilitiesKHR Capabilities;
        std::vector<vk::SurfaceFormatKHR> Formats;
        std::vector<vk::PresentModeKHR> PresentModes;
    };

    class VulkanSwapchain {
    public:

        explicit VulkanSwapchain(Ref<VulkanDevice>& device, vk::SurfaceKHR surface);
        ~VulkanSwapchain();

        void Destroy();
    private:
        Ref<VulkanDevice> m_Device;
        vk::SwapchainKHR m_Swapchain;

        SwapchainSupportDetails m_SupportDetails;
    };

} // HausEngine

