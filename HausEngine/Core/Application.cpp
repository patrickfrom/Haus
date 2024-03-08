#include "Application.h"
#include <iostream>

namespace HausEngine {
    Application::Application() {
        m_Window = Window::Create();
    }

    Application::~Application() {
        std::cout << "Destructed Application" << "\n";
    };

    void Application::Run() {
        while (!glfwWindowShouldClose(m_Window->GetNativeWindow())) {
            m_Window->PollEvents();
        }
    }
} // HausEngine