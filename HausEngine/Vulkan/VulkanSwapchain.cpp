#include "VulkanSwapchain.h"

namespace HausEngine {
    VulkanSwapchain::VulkanSwapchain(Ref<VulkanDevice>& device, vk::SurfaceKHR surface) : m_Device(device) {
        auto logicalDevice = device->GetDevice();
        auto physicalDevice = device->GetPhysicalDevice()->GetPhysicalDevice();

        m_SupportDetails.Capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
        m_SupportDetails.Formats = physicalDevice.getSurfaceFormatsKHR(surface);
        m_SupportDetails.PresentModes = physicalDevice.getSurfacePresentModesKHR(surface);
    }

    VulkanSwapchain::~VulkanSwapchain() = default;

    void VulkanSwapchain::Destroy() {
        m_Device->GetDevice().destroySwapchainKHR(m_Swapchain);
    }
} // HausEngine