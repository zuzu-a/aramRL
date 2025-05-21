#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SDL.h>
#include <entt/entt.hpp>
#include "render.hpp" // Assuming render.hpp is in the same directory or include paths are set
#include "inc/core/systems/InputSystem.hpp"
#include "inc/core/systems/MovementSystem.hpp"
#include "inc/core/systems/TileSystem.hpp"

// Forward declarations are not strictly necessary here as SDL.h includes them.
// struct SDL_Window;
// struct SDL_Renderer;

class Engine {
public:
    Engine();
    ~Engine();

    bool Initialize(); // Handles SDL init, window/renderer creation
    void Run();        // Contains the main game loop
    void Shutdown();   // Handles SDL cleanup

    entt::registry& GetRegistry() { return m_registry; } // Provide access to the registry
    Renderer& GetRenderer() { return m_renderer; }     // Provide access to the renderer

private:
    void ProcessEvents(); // Helper for event handling in the loop
    void Update(float deltaTime);      // Helper for game logic updates
    void Render();       // Helper for rendering operations

    SDL_Window* m_window;
    // SDL_Renderer* m_sdlRenderer; // The Engine will own our Renderer object instead
    Renderer m_renderer; // Renderer object, needs to be default constructible or initialized properly
    InputSystem m_inputSystem;
    MovementSystem m_movementSystem;
    TileSystem m_tileSystem;

    entt::registry m_registry;

    bool m_isRunning;
    // Constants for window size (can be made configurable later)
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    
    // Delta time calculation members
    Uint32 m_lastFrameTime;
};

#endif // ENGINE_HPP
