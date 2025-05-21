#include "inc/core/Engine.hpp" // Adjust path as needed
#include <iostream>

int main(int argc, char* argv[]) {
    Engine engine;

    if (!engine.Initialize()) {
        std::cerr << "Failed to initialize engine." << std::endl;
        return -1;
    }

    engine.Run();
    engine.Shutdown();

    return 0;
}