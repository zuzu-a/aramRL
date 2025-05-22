#include "inc/core/Engine.hpp" // Adjust path as needed
#include <SDL_image.h> // For IMG_Init/Quit
#include <iostream>    // For error messages
#include "inc/core/components/PlayerComponent.hpp"    // For player entity creation
#include "inc/core/components/TransformComponent.hpp" // For player entity creation
#include "inc/core/components/InputComponent.hpp"     // For player entity creation
#include "inc/core/procgen/PointGenerator.hpp"      // For PointGenerator and AABB

// Define FIXED_UPDATE_TIMESTEP if it's a static const in the header
const float Engine::FIXED_UPDATE_TIMESTEP = 1.0f / 60.0f;

Engine::Engine() : 
    m_window(nullptr), 
    m_renderer(nullptr), 
    m_inputSystem(), 
    m_movementSystem(), 
    m_tileSystem(),     
    m_resourceManager(), 
    m_entityFactory(m_registry, m_resourceManager),
    m_dualMeshGenerator(m_entityFactory), // Initialize DualMeshGenerator
    m_gameMap(),                          // Initialize Tilemap
    m_procGenMeshData(),                  // Initialize MeshData
    m_isRunning(false), 
    m_lastFrameTime(0), 
    m_accumulatedTime(0.0f)
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
    m_renderer = Renderer(sdlRendererInstance); 
    m_resourceManager.Initialize(sdlRendererInstance); // Initialize ResourceManager with the SDL_Renderer

    // Create the scene texture for rendering to
    m_sceneTexture = SDL_CreateTexture(sdlRendererInstance, 
                                       SDL_PIXELFORMAT_ARGB8888, 
                                       SDL_TEXTUREACCESS_TARGET, 
                                       SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!m_sceneTexture) {
        std::cerr << "Failed to create scene texture: " << SDL_GetError() << std::endl;
        // Handle error, potentially return false
        SDL_DestroyRenderer(sdlRendererInstance);
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        IMG_Quit();
        SDL_Quit();
        return false;
    }
    // m_ditheredSceneTexture will be created on the fly or also here if preferred static

    // Example Asset Loading
    // if (!m_resourceManager.LoadTexture("assets/textures/player.png", "player_sprite")) {
    //     std::cerr << "Failed to load player texture via ResourceManager" << std::endl;
    // }
    // if (m_resourceManager.LoadTexture("tests/data/dummy_sheet.png", "dummy_tilesheet")) {
    //    if (!m_resourceManager.LoadTileset("tests/data/sample_tileset.json", "dummy_tilesheet", "sample_tileset")) {
    //        std::cerr << "Failed to load sample_tileset via ResourceManager" << std::endl;
    //    }
    // } else {
    //    std::cerr << "Failed to load dummy_tilesheet for tileset." << std::endl;
    // }

    // Load definitions
    if (!m_resourceManager.LoadCharacterDefinition("assets/data/content_definitions/characters/player_hero.json", "player_hero")) {
        std::cerr << "Engine: Failed to load player_hero definition!" << std::endl;
        // Consider returning false or handling this error more gracefully
    }
    if (!m_resourceManager.LoadItemDefinition("assets/data/content_definitions/items/health_potion.json", "health_potion")) {
        std::cerr << "Engine: Failed to load health_potion definition!" << std::endl;
    }
    // Example: Load a TilesetDefinition (ensure the JSON and texture are set up)
    // if (!m_resourceManager.LoadTilesetDefinition("assets/data/content_definitions/tilesets/outdoor_definition.json", "outdoor_tiles_def")) {
    //    std::cerr << "Engine: Failed to load outdoor_tiles_def!" << std::endl;
    // }
    // Then, to load the actual Tileset object using the definition:
    // if (m_resourceManager.GetTilesetDefinition("outdoor_tiles_def")) { // Check if def loaded
    //    if (!m_resourceManager.LoadTilesetFromDefinition("outdoor_tiles_def")) {
    //        std::cerr << "Engine: Failed to load tileset from definition 'outdoor_tiles_def'!" << std::endl;
    //    }
    // }
    // Load the dummy texture for the tileset
    if (!m_resourceManager.LoadTexture("tests/data/dummy_sheet.png", "sheet_for_tileset")) {
        std::cerr << "Engine: Failed to load dummy_sheet.png for world_tileset!" << std::endl;
        // Potentially return false or handle error
    }
    // Load the sample tileset using the dummy texture
    if (!m_resourceManager.LoadTileset("tests/data/sample_tileset.json", "sheet_for_tileset", "world_tileset")) {
        std::cerr << "Engine: Failed to load world_tileset!" << std::endl;
        // Potentially return false or handle error
    }

    // Procedural Generation
    int64_t mapSeed = 12345; 
    ProcGen::AABB mapBounds = {0.0f, 0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)};
    float pointSpacing = 50.0f; 
    
    ProcGen::PointGenerator pointGenerator; // Create local or member if it has state
    m_procGenMeshData.bounds = mapBounds; 
    m_procGenMeshData.sites = pointGenerator.GeneratePoissonPoints(mapBounds, pointSpacing, 30, static_cast<unsigned int>(mapSeed));
    
    m_dualMeshGenerator.GenerateDelaunay(m_procGenMeshData); 
    m_dualMeshGenerator.GenerateMapFeatures(m_procGenMeshData, mapSeed); 

    // Populate Tilemap
    // Assuming 32x32 tiles from sample_tileset.json (which has tilewidth: 32, tileheight: 32)
    Tileset* worldTileset = m_resourceManager.GetTileset("world_tileset");
    int tileWidth = worldTileset ? worldTileset->getTileWidth() : 32;
    int tileHeight = worldTileset ? worldTileset->getTileHeight() : 32;
    if (tileWidth == 0) tileWidth = 32; // Fallback if tileset not loaded or has 0 width
    if (tileHeight == 0) tileHeight = 32;

    int mapGridWidth = SCREEN_WIDTH / tileWidth; 
    int mapGridHeight = SCREEN_HEIGHT / tileHeight;
    m_gameMap.PopulateFromMeshData(m_procGenMeshData, mapGridWidth, mapGridHeight);


    // Create player entity using EntityFactory
    entt::entity player = m_entityFactory.CreateCharacter("player_hero", {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f});
    if (player == entt::null) {
        std::cerr << "Engine: Failed to create player entity!" << std::endl;
        // Potentially return false or handle error
    } else {
        std::cout << "Player entity created successfully by EntityFactory." << std::endl;
    }
    
    // Example: Create an item using EntityFactory
    // entt::entity item = m_entityFactory.CreateItem("health_potion", {100.0f, 100.0f});
    // if (item == entt::null) {
    //    std::cerr << "Engine: Failed to create item 'health_potion'!" << std::endl;
    // }

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
    
    m_lastFrameTime = SDL_GetTicks(); // Ensure m_lastFrameTime is set before the first deltaTime calculation

    while (m_isRunning) {
        Uint32 currentFrameStartTime = SDL_GetTicks();
        float deltaTime = (currentFrameStartTime - m_lastFrameTime) / 1000.0f; // Delta time in seconds
        m_lastFrameTime = currentFrameStartTime;

        m_accumulatedTime += deltaTime;

        ProcessEvents(); // Process all pending events

        // Fixed update loop
        while (m_accumulatedTime >= FIXED_UPDATE_TIMESTEP) {
            UpdateLogic(FIXED_UPDATE_TIMESTEP);
            m_accumulatedTime -= FIXED_UPDATE_TIMESTEP;
        }

        // Pass m_accumulatedTime / FIXED_UPDATE_TIMESTEP as alpha for interpolation if needed in Render
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

void Engine::UpdateLogic(float fixedDeltaTime) { // Renamed from Update, parameter changed
    m_inputSystem.PrepareForNewFrame(m_registry); // Doesn't use deltaTime
    m_inputSystem.UpdateContinuousInput(m_registry); // Doesn't use deltaTime directly
    m_movementSystem.Update(m_registry, fixedDeltaTime); // Uses fixedDeltaTime

    // Game logic updates using m_registry and fixedDeltaTime
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
    SDL_Renderer* currentSdlRenderer = m_renderer.GetSDLRenderer();

    // 1. Render scene to m_sceneTexture
    SDL_SetRenderTarget(currentSdlRenderer, m_sceneTexture);
    m_renderer.SetDrawColor(20, 20, 80, 255); // Dark blue background for the scene texture
    m_renderer.Clear();

    // Render the tilemap first
    Tileset* worldTileset = m_resourceManager.GetTileset("world_tileset");
    if (worldTileset) {
        m_tileSystem.Update(m_gameMap, *worldTileset, m_renderer, m_accumulatedTime / FIXED_UPDATE_TIMESTEP);
    } else {
        // std::cerr << "Engine::Render - world_tileset not found!" << std::endl;
        // Optionally draw a fallback grid or error color if tileset isn't loaded
        m_renderer.SetDrawColor(255,0,255,255); // Bright Pink for error
        m_renderer.Clear(); // Fill scene texture with error color
    }
    
    // Then render sprites/entities on top
    m_renderSystem.Update(m_registry, m_renderer, m_accumulatedTime / FIXED_UPDATE_TIMESTEP); 
    
    // 2. Read pixels from m_sceneTexture to a surface
    SDL_Surface* sceneSurface = SDL_CreateRGBSurfaceWithFormat(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);
    if (sceneSurface) {
        // Important: Ensure render target is m_sceneTexture before reading
        SDL_RenderReadPixels(currentSdlRenderer, nullptr, SDL_PIXELFORMAT_ARGB8888, sceneSurface->pixels, sceneSurface->pitch);

        // 3. Create a destination surface for dithering
        SDL_Surface* ditheredSurface = SDL_CreateRGBSurfaceWithFormat(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);
        if (ditheredSurface) {
            // 4. Apply dithering
            RetroDither::ApplyBayerDithering(sceneSurface, ditheredSurface, 4); // Use 4x4 matrix

            // 5. Create/Update m_ditheredSceneTexture from ditheredSurface
            if (m_ditheredSceneTexture) {
                SDL_DestroyTexture(m_ditheredSceneTexture); // Destroy old one
            }
            m_ditheredSceneTexture = SDL_CreateTextureFromSurface(currentSdlRenderer, ditheredSurface);
            SDL_FreeSurface(ditheredSurface);
        } else {
            std::cerr << "Failed to create dithered surface: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(sceneSurface);
    } else {
        std::cerr << "Failed to create scene surface for reading pixels: " << SDL_GetError() << std::endl;
    }

    // 6. Render final texture to screen
    SDL_SetRenderTarget(currentSdlRenderer, nullptr); // Reset to default render target (the window)
    m_renderer.SetDrawColor(0, 0, 0, 255); // Clear window to black (or other background)
    m_renderer.Clear();

    if (m_ditheredSceneTexture) {
        SDL_RenderCopy(currentSdlRenderer, m_ditheredSceneTexture, nullptr, nullptr);
    } else if (m_sceneTexture) { // Fallback if dithering failed
        SDL_RenderCopy(currentSdlRenderer, m_sceneTexture, nullptr, nullptr);
    }
    
    m_renderer.Present(); // Present the final rendered screen
}

void Engine::Shutdown() {
    std::cout << "Engine Shutting down..." << std::endl;

    if (m_sceneTexture) {
        SDL_DestroyTexture(m_sceneTexture);
        m_sceneTexture = nullptr;
    }
    if (m_ditheredSceneTexture) {
        SDL_DestroyTexture(m_ditheredSceneTexture);
        m_ditheredSceneTexture = nullptr;
    }
    
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
