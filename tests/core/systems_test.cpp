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

// MockRenderer is now in test_fixture.hpp as SDLTestFixture::TestMockRenderer


TEST(TileSystemTest, CanCreateAndUpdate) {
    entt::registry registry;
    TileSystem tileSystem;
    // MockRenderer mockRenderer; // Use a mock or a real SDL_Renderer if available/simple
    SDLTestFixture::TestMockRenderer mockRenderer; // Use the one from the fixture
    
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


// RenderSystem Tests
#include "inc/core/systems/RenderSystem.hpp"
#include "inc/core/components/SpriteComponent.hpp"
#include "test_fixture.hpp" // For SDLTestFixture and TestMockRenderer

// Use the SDLTestFixture to get a valid SDL_Renderer for creating textures if needed
// but RenderSystem itself will be tested with the TestMockRenderer.
class RenderSystemTest : public SDLTestFixture {
protected:
    // SDL_Texture* dummyTexture = nullptr; // Create a dummy texture for tests

    // void SetUp() override {
    //     SDLTestFixture::SetUp();
    //     // Create a minimal 1x1 white texture for testing
    //     SDL_Surface* surface = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);
    //     ASSERT_NE(surface, nullptr);
    //     SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
    //     dummyTexture = SDL_CreateTextureFromSurface(renderer, surface); // renderer from SDLTestFixture
    //     ASSERT_NE(dummyTexture, nullptr);
    //     SDL_FreeSurface(surface);
    // }

    // void TearDown() override {
    //     if (dummyTexture) SDL_DestroyTexture(dummyTexture);
    //     SDLTestFixture::TearDown();
    // }
};


TEST_F(RenderSystemTest, RendersVisibleSprite) {
    entt::registry registry;
    RenderSystem renderSystem;
    SDLTestFixture::TestMockRenderer mockRenderer; // Use the mock from the fixture

    // Create a dummy texture (can be nullptr for this test if DrawTexture handles it,
    // or a real minimal texture if strictly needed by DrawTexture internals not mocked).
    // For this test, RenderSystem::Update checks for sprite.texture, so it should be non-null.
    // The actual texture content doesn't matter as DrawTexture is mocked.
    SDL_Texture* testTexture = (SDL_Texture*)0x1; // Dummy non-null pointer

    auto entity = registry.create();
    auto& transform = registry.emplace<TransformComponent>(entity, Vector2D{100.0f, 100.0f});
    auto& sprite = registry.emplace<SpriteComponent>(entity, testTexture, SDL_Rect{0,0,32,32});
    sprite.visible = true;

    renderSystem.Update(registry, mockRenderer, 0.0f);

    ASSERT_EQ(mockRenderer.drawTextureCalls.size(), 1);
    if (!mockRenderer.drawTextureCalls.empty()) {
        const auto& call = mockRenderer.drawTextureCalls[0];
        ASSERT_EQ(call.texture, testTexture);
        ASSERT_EQ(call.dstRect.x, 100 - (32 * 0.5f)); // Assuming pivot 0.5,0.5
        ASSERT_EQ(call.dstRect.y, 100 - (32 * 0.5f));
        ASSERT_EQ(call.dstRect.w, 32);
        ASSERT_EQ(call.dstRect.h, 32);
    }
}

TEST_F(RenderSystemTest, DoesNotRenderInvisibleSprite) {
    entt::registry registry;
    RenderSystem renderSystem;
    SDLTestFixture::TestMockRenderer mockRenderer;

    SDL_Texture* testTexture = (SDL_Texture*)0x1; 
    auto entity = registry.create();
    registry.emplace<TransformComponent>(entity);
    auto& sprite = registry.emplace<SpriteComponent>(entity, testTexture, SDL_Rect{0,0,32,32});
    sprite.visible = false; // Invisible

    renderSystem.Update(registry, mockRenderer, 0.0f);
    ASSERT_TRUE(mockRenderer.drawTextureCalls.empty());
}

TEST_F(RenderSystemTest, ZOrderSorting) {
    entt::registry registry;
    RenderSystem renderSystem;
    SDLTestFixture::TestMockRenderer mockRenderer;

    SDL_Texture* tex1 = (SDL_Texture*)0x1;
    SDL_Texture* tex2 = (SDL_Texture*)0x2;

    // Entity 1: z_order 1
    auto entity1 = registry.create();
    registry.emplace<TransformComponent>(entity1, Vector2D{10.0f, 10.0f});
    auto& sprite1 = registry.emplace<SpriteComponent>(entity1, tex1, SDL_Rect{0,0,16,16});
    sprite1.z_order = 1;

    // Entity 2: z_order 0
    auto entity2 = registry.create();
    registry.emplace<TransformComponent>(entity2, Vector2D{20.0f, 20.0f}); // Different Y for sort stability check
    auto& sprite2 = registry.emplace<SpriteComponent>(entity2, tex2, SDL_Rect{0,0,16,16});
    sprite2.z_order = 0;
    
    renderSystem.Update(registry, mockRenderer, 0.0f);

    ASSERT_EQ(mockRenderer.drawTextureCalls.size(), 2);
    if (mockRenderer.drawTextureCalls.size() == 2) {
        ASSERT_EQ(mockRenderer.drawTextureCalls[0].texture, tex2); // Entity 2 (z_order 0) should be drawn first
        ASSERT_EQ(mockRenderer.drawTextureCalls[1].texture, tex1); // Entity 1 (z_order 1) should be drawn second
    }
}

TEST_F(RenderSystemTest, YSortForSameZOrder) {
    entt::registry registry;
    RenderSystem renderSystem;
    SDLTestFixture::TestMockRenderer mockRenderer;

    SDL_Texture* texA = (SDL_Texture*)0xA;
    SDL_Texture* texB = (SDL_Texture*)0xB;

    // Entity A: y = 100, z_order = 0
    auto entityA = registry.create();
    registry.emplace<TransformComponent>(entityA, Vector2D{50.0f, 100.0f});
    auto& spriteA = registry.emplace<SpriteComponent>(entityA, texA, SDL_Rect{0,0,16,16});
    spriteA.z_order = 0;

    // Entity B: y = 50, z_order = 0
    auto entityB = registry.create();
    registry.emplace<TransformComponent>(entityB, Vector2D{50.0f, 50.0f});
    auto& spriteB = registry.emplace<SpriteComponent>(entityB, texB, SDL_Rect{0,0,16,16});
    spriteB.z_order = 0;

    renderSystem.Update(registry, mockRenderer, 0.0f);

    ASSERT_EQ(mockRenderer.drawTextureCalls.size(), 2);
    if (mockRenderer.drawTextureCalls.size() == 2) {
        ASSERT_EQ(mockRenderer.drawTextureCalls[0].texture, texB); // Entity B (y=50) drawn first
        ASSERT_EQ(mockRenderer.drawTextureCalls[1].texture, texA); // Entity A (y=100) drawn second
    }
}
