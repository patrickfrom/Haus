//
// Created by bauhaus on 08-03-24.
//

#ifndef HAUS_WINDOW_H
#define HAUS_WINDOW_H

#include <GLFW/glfw3.h>
#include <iostream>

namespace Haus {

    class Window {
    public:
        explicit Window(const std::string& name);
        ~Window();

        GLFWwindow* GetNativeWindow() {
            return m_NativeWindow;
        }
    private:
        std::string m_Name;
        GLFWwindow* m_NativeWindow;
    };

} // Haus

#endif //HAUS_WINDOW_H
