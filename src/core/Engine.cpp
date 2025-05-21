#include "inc/core/Engine.hpp" // Adjust path as needed
#include <SDL_image.h> // For IMG_Init/Quit
#include <iostream>    // For error messages
#include "inc/core/components/PlayerComponent.hpp"    // For player entity creation
#include "inc/core/components/TransformComponent.hpp" // For player entity creation
#include "inc/core/components/InputComponent.hpp"     // For player entity creation

Engine::Engine() : 
    m_window(nullptr), 
    m_renderer(nullptr), 
    m_inputSystem(), 
    m_movementSystem(), // Initialize MovementSystem
    m_tileSystem(),     // Initialize TileSystem
    m_isRunning(false), 
    m_lastFrameTime(0) 
{
    // Constructor: m_renderer is initialized using its Renderer(nullptr) constructor,
    // actual SDL_Renderer will be set in Initialize.
}

Engine::~Engine() {
    // Shutdown should be called explicitly.
    // If m_window is not null, it means Shutdown was likely not called.
    if (m_window) {
        std::cerr << "Engine destructor called but Shutdown() was not called or did not complete." << std::endl;
        // Attempt a cleanup as a fallback, though this is not ideal.
        Shutdown(); 
    }
}

bool Engine::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        // Depending on how critical image loading is, you might want to handle this more strictly.
    }

    m_window = SDL_CreateWindow(
        "Aram Engine",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!m_window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit(); // Clean up SDL if window creation fails
        return false;
    }

    SDL_Renderer* sdlRendererInstance = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdlRendererInstance) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        IMG_Quit(); // Clean up SDL_image
        SDL_Quit(); // Clean up SDL
        return false;
    }
    
    // Assign the created SDL_Renderer to our Renderer wrapper object
    m_renderer = Renderer(sdlRendererInstance); // This uses Renderer's copy assignment or move assignment if available and applicable.
                                                // Assumes Renderer has a working copy/move assignment or a constructor that can re-initialize.
                                                // If Renderer is not designed for reassignment, this might need adjustment
                                                // (e.g. m_renderer.SetSDLRenderer(sdlRendererInstance);)

    // Create a player entity
    auto playerEntity = m_registry.create();
    m_registry.emplace<PlayerComponent>(playerEntity, 0); // Player ID 0
    m_registry.emplace<TransformComponent>(playerEntity, Vector2D{SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f});
    m_registry.emplace<InputComponent>(playerEntity);
    std::cout << "Player entity created with InputComponent." << std::endl; // For debugging

    m_isRunning = true;
    m_lastFrameTime = SDL_GetTicks();
    std::cout << "Engine Initialized Successfully." << std::endl;
    return true;
}

void Engine::Run() {
    if (!m_isRunning) {
        std::cerr << "Engine cannot run because it's not initialized or has been shut down." << std::endl;
        return;
    }
    std::cout << "Engine Run loop started." << std::endl;
    while (m_isRunning) {
        Uint32 currentFrameTime = SDL_GetTicks();
        float deltaTime = (currentFrameTime - m_lastFrameTime) / 1000.0f; // Delta time in seconds
        m_lastFrameTime = currentFrameTime;

        ProcessEvents();
        Update(deltaTime);
        Render();
    }
    std::cout << "Engine Run loop ended." << std::endl;
}

void Engine::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_isRunning = false;
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            std::cout << "Window resized to " << event.window.data1 << "x" << event.window.data2 << std::endl;
            // TODO: The Renderer might need to be explicitly told about the resize,
            // or handle it internally if SDL_Renderer does it automatically.
        }
        // Pass events to InputSystem
        m_inputSystem.ProcessEvent(m_registry, event);
    }
}

void Engine::Update(float deltaTime) {
    m_inputSystem.PrepareForNewFrame(m_registry); // Call before UpdateContinuousInput
    m_inputSystem.UpdateContinuousInput(m_registry);
    m_movementSystem.Update(m_registry, deltaTime);

    // Game logic updates using m_registry and deltaTime
    // For example, other systems would be called here:
    // collisionSystem.update(m_registry, deltaTime);
    // std::cout << "Update delta time: " << deltaTime << std::endl;

    // Debug: Print player position after movement
    auto player_view_debug = m_registry.view<PlayerComponent, TransformComponent>();
    for (auto entity_debug : player_view_debug) {
        const auto& transform_debug = player_view_debug.get<TransformComponent>(entity_debug);
        // std::cout << "Player Position: (" << transform_debug.position.x << ", " << transform_debug.position.y << ")" << std::endl;
    }
}

void Engine::Render() {
    m_renderer.SetDrawColor(20, 20, 80, 255); // Dark blue background
    m_renderer.Clear();

    m_tileSystem.Update(m_registry, m_renderer, 0.0f); // deltaTime might not be needed for rendering

    // Render player
    auto player_view = m_registry.view<PlayerComponent, TransformComponent>();
    for (auto entity : player_view) {
        const auto& transform = player_view.get<TransformComponent>(entity);
        m_renderer.SetDrawColor(100, 255, 100, 255); // Light Green for player
        // DrawChar expects int coordinates
        m_renderer.DrawChar('@', static_cast<int>(transform.position.x), static_cast<int>(transform.position.y));
    }

    m_renderer.Present();
}

void Engine::Shutdown() {
    std::cout << "Engine Shutting down..." << std::endl;
    
    // Get the SDL_Renderer from our Renderer wrapper to destroy it
    SDL_Renderer* sdlRendererInstance = m_renderer.GetSDLRenderer();
    if (sdlRendererInstance) {
        SDL_DestroyRenderer(sdlRendererInstance);
        // After destroying, it's good practice to nullify the pointer in our wrapper,
        // though the wrapper itself will be destroyed with the Engine.
        // m_renderer.SetSDLRenderer(nullptr); // If such a method exists
    }

    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    IMG_Quit();
    SDL_Quit();
    
    m_isRunning = false; // Ensure isRunning is false after shutdown
    std::cout << "Engine Shutdown complete." << std::endl;
}
