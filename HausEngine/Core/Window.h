#pragma once

#include <GLFW/glfw3.h>
#include <memory>

namespace HausEngine {

    class Window {
    public:
        Window();
        ~Window();

        void Shutdown();

        void PollEvents();

        GLFWwindow* GetNativeWindow() {
            return m_NativeWindow;
        }


        static std::unique_ptr<Window> Create();
    private:
        GLFWwindow* m_NativeWindow;
    };

} // HausEngine