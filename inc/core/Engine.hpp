#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SDL.h>
#include <entt/entt.hpp>
#include "render.hpp" // Assuming render.hpp is in the same directory or include paths are set
#include "inc/core/systems/InputSystem.hpp"
#include "inc/core/systems/MovementSystem.hpp"
#include "inc/core/systems/TileSystem.hpp"
#include "inc/core/systems/RenderSystem.hpp" 
#include "inc/core/ResourceManager.hpp"
#include "inc/core/EntityFactory.hpp" 
#include "inc/core/RetroDither.hpp" // Added RetroDither header

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

    entt::registry& GetRegistry() { return m_registry; } 
    const entt::registry& GetRegistry() const { return m_registry; } // Const version
    Renderer& GetRenderer() { return m_renderer; }
    const Renderer& GetRenderer() const { return m_renderer; } // Const version

    static const float FIXED_UPDATE_TIMESTEP; // Added for fixed timestep

private:
    void ProcessEvents(); // Helper for event handling in the loop
    void UpdateLogic(float fixedDeltaTime);      // Renamed from Update
    void Render();       // Helper for rendering operations (non-const as it calls non-const Renderer methods)

    SDL_Window* m_window = nullptr; // Initialize to nullptr
    // SDL_Renderer* m_sdlRenderer; // The Engine will own our Renderer object instead
    Renderer m_renderer; // Renderer object, needs to be default constructible or initialized properly
    InputSystem m_inputSystem;
    MovementSystem m_movementSystem;
    TileSystem m_tileSystem;
    ResourceManager m_resourceManager;
    EntityFactory m_entityFactory;
    RenderSystem m_renderSystem; // Added RenderSystem member

    entt::registry m_registry;

    bool m_isRunning;
    // Constants for window size (can be made configurable later)
    static constexpr int SCREEN_WIDTH = 800;
    static constexpr int SCREEN_HEIGHT = 600;
    
    // Delta time calculation members
    Uint32 m_lastFrameTime = 0; // Initialize
    float m_accumulatedTime = 0.0f; // Added for fixed timestep accumulation

    // Textures for post-processing effects
    SDL_Texture* m_sceneTexture = nullptr;
    SDL_Texture* m_ditheredSceneTexture = nullptr;
};

#endif // ENGINE_HPP
