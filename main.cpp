#include <HausEngine.h>
#include <iostream>

int main() {
    try {
        HausEngine::Application App;
        App.Run();
    } catch (std::exception& exception) {
        std::cerr << exception.what() << "\n";
    }

    return 0;
}