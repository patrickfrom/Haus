#include <iostream>
#include "Application.h"

int main() {
    Haus::ApplicationSpecification specification{
            .Name = "Haus",
            .Width = 800,
            .Height = 600,
    };

    Haus::Application app{specification};

    try {
        app.SetClearColor(glm::vec4(0.5f, 0.25f, 1.0f, 1.0f));
        app.Run();
    } catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
    }

    app.Shutdown();
    return 0;
}