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
        app.Run();
    } catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
    }

    app.Shutdown();
    return 0;
}