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
    }

    Window::~Window() {
        std::cout << "Shutting down Window" << "\n";
        Shutdown();
    }

    void Window::Shutdown() {
        m_RendererContext->GetDevice()->Destroy();
        glfwTerminate();
    }

    void Window::PollEvents() {
        glfwPollEvents();
    }

    std::unique_ptr<Window> Window::Create() {
        return std::make_unique<Window>();
    }
} // HausEngine