#include "inc/core/systems/TileSystem.hpp" // Adjust path
#include "inc/core/render.hpp" // For Renderer class
#include <iostream>

TileSystem::TileSystem() {}
TileSystem::~TileSystem() {}

void TileSystem::Update(entt::registry& registry, Renderer& renderer, float deltaTime) {
    // std::cout << "TileSystem::Update called (TODO: Implement tile rendering)" << std::endl;
    // Placeholder: Draw a grid or some simple tile pattern if desired for basic visual testing
    // For example, draw some grid lines:
    renderer.SetDrawColor(50, 50, 50, 255); // Dark grey
    for (int x = 0; x < 800; x += 32) { // Assuming 800 is screen width from Engine
        // renderer.DrawRect(x, 0, 1, 600, false); // Vertical line (x, y, w, h) // Corrected to DrawLine
        renderer.DrawRect(x, 0, 1, 600, true); // Using DrawRect to simulate a line for now
    }
    for (int y = 0; y < 600; y += 32) { // Assuming 600 is screen height from Engine
       // renderer.DrawRect(0, y, 800, 1, false); // Horizontal line // Corrected to DrawLine
       renderer.DrawRect(0, y, 800, 1, true); // Using DrawRect to simulate a line for now
    }
}
