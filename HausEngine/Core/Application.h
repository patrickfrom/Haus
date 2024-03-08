#pragma once

#include "Window.h"
#include <GLFW/glfw3.h>
#include <memory>

namespace HausEngine {
    class Application {
    public:
        Application();
        ~Application();

        void Run();
    private:
        std::unique_ptr<Window> m_Window;
    };

} // HausEngine
