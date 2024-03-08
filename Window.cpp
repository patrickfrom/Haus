//
// Created by bauhaus on 08-03-24.
//

#include "Window.h"

namespace Haus {
    Window::Window(const std::string& name) : m_Name(name) {
        if (!glfwInit())
            throw std::runtime_error("Failed to Initialize GLFW");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_NativeWindow = glfwCreateWindow(800, 600, name.c_str(), nullptr, nullptr);
        if (!m_NativeWindow)
            throw std::runtime_error("Failed to create GLFW Window");
    }

    Window::~Window() {
        glfwDestroyWindow(m_NativeWindow);
        glfwTerminate();
    }
} // Haus