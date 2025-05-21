#include <gtest/gtest.h>
#include "inc/core/systems/InputSystem.hpp" // Direct include for testing
#include "inc/core/components/InputComponent.hpp"
#include "inc/core/components/PlayerComponent.hpp"
#include <entt/entt.hpp> // Direct include for entt::registry
#include <SDL.h> // For SDL_Event and SDL key codes

TEST(InputSystemTest, AttackAction) {
    entt::registry registry;
    InputSystem inputSystem;

    auto player = registry.create();
    registry.emplace<InputComponent>(player);
    registry.emplace<PlayerComponent>(player); // InputSystem currently views for PlayerComponent too

    SDL_Event event;
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_SPACE;
    event.key.repeat = 0;

    inputSystem.PrepareForNewFrame(registry); // Reset action_triggered
    inputSystem.ProcessEvent(registry, event);

    const auto& inputComp = registry.get<InputComponent>(player);
    ASSERT_EQ(inputComp.current_action, InputAction::ATTACK);
    ASSERT_TRUE(inputComp.action_triggered);
}

// TODO: Add tests for movement input (requires mocking SDL_GetKeyboardState or a more advanced setup)
// For now, we will focus on the event-based input.
// A possible approach for movement tests would involve:
// 1. An interface for keyboard state querying.
// 2. A mock implementation of this interface for tests.
// 3. InputSystem uses this interface instead of SDL_GetKeyboardState directly.

// Test to ensure PrepareForNewFrame resets action_triggered
TEST(InputSystemTest, PrepareForNewFrameResetsAction) {
    entt::registry registry;
    InputSystem inputSystem;

    auto player = registry.create();
    auto& inputComp = registry.emplace<InputComponent>(player);
    registry.emplace<PlayerComponent>(player);

    // Simulate a triggered action from a previous frame
    inputComp.current_action = InputAction::ATTACK;
    inputComp.action_triggered = true;

    inputSystem.PrepareForNewFrame(registry);

    ASSERT_FALSE(inputComp.action_triggered);
    // current_action can remain as the last action, that's fine.
    // The key is that action_triggered is false.
}


// MovementSystem Tests
#include "inc/core/systems/MovementSystem.hpp"
#include "inc/core/components/TransformComponent.hpp"
// InputComponent and PlayerComponent are already included

TEST(MovementSystemTest, MovesEntityWithInput) {
    entt::registry registry;
    MovementSystem movementSystem;

    auto entity = registry.create();
    auto& transform = registry.emplace<TransformComponent>(entity, Vector2D{100.0f, 100.0f});
    auto& input = registry.emplace<InputComponent>(entity);
    registry.emplace<PlayerComponent>(entity); // Add PlayerComponent as MovementSystem's view requires it

    input.movement_direction = {1.0f, 0.0f}; // Move right

    float initial_x = transform.position.x;
    float deltaTime = 0.1f; // 100ms
    
    movementSystem.Update(registry, deltaTime);

    // Assuming baseSpeed in MovementSystem is 200.0f
    // Expected movement = 1.0f * 200.0f * 0.1f = 20.0f
    ASSERT_FLOAT_EQ(transform.position.x, initial_x + 20.0f);
    ASSERT_FLOAT_EQ(transform.position.y, 100.0f); // No vertical input
}

// TileSystem Tests
#include "inc/core/systems/TileSystem.hpp"
#include "inc/core/render.hpp" // For Renderer mock/stub if needed, or real for integration
// For a stub test, we might not need a full Renderer, but TileSystem::Update expects one.

// Mock Renderer for TileSystem test (very basic)
class MockRenderer : public Renderer {
public:
    MockRenderer() : Renderer(nullptr) {} // Call base constructor with nullptr
    int drawRectCallCount = 0;
    void DrawRect(int x, int y, int w, int h, bool fill) override {
        drawRectCallCount++;
        // Call base or do nothing, just record call for test
        // For this test, we're just checking if TileSystem::Update can be called.
    }
    void SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) override {
        // Do nothing
    }
};


TEST(TileSystemTest, CanCreateAndUpdate) {
    entt::registry registry;
    TileSystem tileSystem;
    MockRenderer mockRenderer; // Use a mock or a real SDL_Renderer if available/simple
    
    // This test mainly checks that Update can be called without crashing.
    // More complex tests would verify actual rendering logic.
    ASSERT_NO_THROW(tileSystem.Update(registry, mockRenderer, 0.1f));
    
    // If TileSystem::Update had specific drawing calls we could count:
    // For example, if the grid drawing in TileSystem::Update is uncommented:
    // It draws 800/32 vertical lines + 600/32 horizontal lines
    // = 25 vertical + 18.75 (so 18 or 19) horizontal lines.
    // Let's assume 25 vertical + 19 horizontal = 44 calls to DrawRect.
    // This depends on the exact implementation of TileSystem's stub.
    // For the current stub which has the drawing commented out, drawRectCallCount would be 0.
    // If uncommented, it would be:
    // int expected_draw_calls = (800/32) + (600/32); // approx 25 + 18 = 43
    // EXPECT_EQ(mockRenderer.drawRectCallCount, expected_draw_calls);
    // For the current stub where drawing is commented out:
    EXPECT_EQ(mockRenderer.drawRectCallCount, 0); 
    // If drawing is uncommented in TileSystem.cpp (25 vertical, 19 horizontal lines):
    // EXPECT_EQ(mockRenderer.drawRectCallCount, 25 + 19); // 44
}
