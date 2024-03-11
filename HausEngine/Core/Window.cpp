#include "Window.h"
#include <iostream>

namespace HausEngine {
    Window::Window() {
        if (!glfwInit())
            throw std::runtime_error("Failed to Initialize GLFW");

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_NativeWindow = glfwCreateWindow(800, 600, "HausEngine", nullptr, nullptr);
        if (!m_NativeWindow)
            throw std::runtime_error("Failed to create GLFW Window");


        m_RendererContext = Ref<VulkanContext>::Create();

        if (glfwCreateWindowSurface(VulkanContext::GetInstance(), m_NativeWindow, nullptr, reinterpret_cast<VkSurfaceKHR*>(&m_Surface)) != VK_SUCCESS)
            throw std::runtime_error("Failed to create Vulkan Surface");

        m_Swapchain = Ref<VulkanSwapchain>::Create(m_RendererContext->GetDevice(), m_Surface);
    }

    Window::~Window() {
        std::cout << "Shutting down Window" << "\n";
        Shutdown();
    }

    void Window::Shutdown() {
        m_Swapchain->Destroy();
        m_RendererContext->GetDevice()->Destroy();
        m_RendererContext->GetInstance().destroySurfaceKHR(m_Surface);
        glfwTerminate();
    }

    void Window::PollEvents() {
        glfwPollEvents();
    }

    std::unique_ptr<Window> Window::Create() {
        return std::make_unique<Window>();
    }
} // HausEngine