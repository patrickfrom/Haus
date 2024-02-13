#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>

namespace Haus {
    struct ApplicationSpecification {
        std::string Name;
        int Width;
        int Height;
    };

    class Application {
    public:
        explicit Application(ApplicationSpecification& specification);

        void Run();
        void Shutdown();
    private:
        ApplicationSpecification m_Specification;

        void Loop();
    private:
        void InitWindow();
        void CleanupGLFW();

        GLFWwindow* m_Window;
    private:
        void InitVulkan();
        void CreateInstance();

        void CleanupVulkan();

        vk::Instance m_Instance;
    };

} // Haus
