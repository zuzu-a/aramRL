#include <gtest/gtest.h>
#include "inc/core/EntityFactory.hpp"
#include "inc/core/ResourceManager.hpp"
#include "inc/core/components/TransformComponent.hpp"
#include "inc/core/components/PlayerComponent.hpp"
#include "inc/core/components/InputComponent.hpp"
#include "inc/core/components/SpriteComponent.hpp" // Added SpriteComponent
#include "test_fixture.hpp" // For SDLTestFixture and TestUtils::CreateDummyFile

class EntityFactoryTest : public SDLTestFixture {
protected:
    entt::registry registry;
    ResourceManager rm; // Real RM instance
    EntityFactory factory;

    EntityFactoryTest() : factory(registry, rm) {}

    void SetUp() override {
        SDLTestFixture::SetUp();
        rm.Initialize(renderer); // Initialize RM with the renderer from the fixture
        
        // Create dummy player_hero.json for tests
        std::string playerJsonContent = R"({
            "id": "test_player",
            "friendlyName": "Test Player",
            "textureAlias": "player_tex_alias", 
            "speed": 150.0,
            "health": 120,
            "initialScale": {"x": 1.1, "y": 1.1},
            "componentsToAdd": [
                {"name": "PlayerComponent", "properties": {"player_id": 7, "is_active": true}},
                {"name": "InputComponent", "properties": {}}, 
                {"name": "TransformComponent", "properties": {"rotation": 10.0}} 
            ]
        })";
        TestUtils::CreateDummyFile("tests/data/test_player_def.json", playerJsonContent);
        ASSERT_TRUE(rm.LoadCharacterDefinition("tests/data/test_player_def.json", "test_player"));

        // Create dummy item.json for tests
        std::string itemJsonContent = R"({
            "id": "test_item",
            "friendlyName": "Test Item",
            "description": "A simple test item.",
            "textureAlias": "item_tex_alias",
            "componentsToAdd": []
        })";
        TestUtils::CreateDummyFile("tests/data/test_item_def.json", itemJsonContent);
        ASSERT_TRUE(rm.LoadItemDefinition("tests/data/test_item_def.json", "test_item"));
        
        // WORKER NOTE: For tests involving SpriteComponent (Phase 2), ensure dummy textures are "loaded"
        // For now, EntityFactory doesn't add SpriteComponent, so texture presence is not critical for these tests.
        // Example: rm.LoadTexture("tests/data/dummy_texture.png", "player_tex_alias"); 
        //          rm.LoadTexture("tests/data/dummy_texture.png", "item_tex_alias");
        
        // Load a dummy texture for sprite tests
        // WORKER NOTE: This test requires "tests/data/dummy_texture.png" to be manually created.
        // If not present, LoadTexture will return nullptr, and tests for SpriteComponent.texture might reflect that.
        rm.LoadTexture("tests/data/dummy_texture.png", "player_tex_alias"); 
        rm.LoadTexture("tests/data/dummy_texture.png", "item_tex_alias"); 
    }
};

TEST_F(EntityFactoryTest, CreateCharacterValid) {
    Vector2D initialPos = {100.0f, 200.0f};
    entt::entity player = factory.CreateCharacter("test_player", initialPos);

    ASSERT_NE(player, entt::null);
    ASSERT_TRUE(registry.valid(player));

    // Check TransformComponent (always added, position and scale from def)
    ASSERT_TRUE(registry.all_of<TransformComponent>(player));
    const auto& transform = registry.get<TransformComponent>(player);
    ASSERT_FLOAT_EQ(transform.position.x, initialPos.x);
    ASSERT_FLOAT_EQ(transform.position.y, initialPos.y);
    ASSERT_FLOAT_EQ(transform.scale.x, 1.1f); // From test_player_def.json
    ASSERT_FLOAT_EQ(transform.scale.y, 1.1f); // From test_player_def.json
    // Rotation should be from JSON if specified, otherwise default 0
    // The test_player_def.json's TransformComponent properties section will override initialScale from def root
    ASSERT_FLOAT_EQ(transform.rotation, 10.0f); 


    // Check components added via componentsToAdd list in JSON
    ASSERT_TRUE(registry.all_of<PlayerComponent>(player));
    const auto& playerComp = registry.get<PlayerComponent>(player);
    ASSERT_EQ(playerComp.player_id, 7);
    ASSERT_TRUE(playerComp.is_active);

    ASSERT_TRUE(registry.all_of<InputComponent>(player));
    // InputComponent properties can be checked if they are set from JSON in ApplyComponentData

    // Check SpriteComponent (added by EntityFactory based on textureAlias in def)
    ASSERT_TRUE(registry.all_of<SpriteComponent>(player));
    const auto& sprite = registry.get<SpriteComponent>(player);
    ASSERT_NE(sprite.texture, nullptr); // Should have a texture if "player_tex_alias" was loaded
    ASSERT_EQ(sprite.sourceRect.w, 1); // Default from dummy_texture.png (assuming it's 1x1)
    ASSERT_EQ(sprite.sourceRect.h, 1); // or whatever SDL_QueryTexture returns for it.
                                       // This depends on the actual dummy_texture.png.
                                       // If dummy_texture.png is not loaded, texture will be nullptr.
    if (sprite.texture == nullptr) {
        GTEST_LOG_(INFO) << "SpriteComponent.texture is null in CreateCharacterValid, dummy_texture.png likely not loaded.";
    } else {
        int w, h;
        SDL_QueryTexture(sprite.texture, nullptr, nullptr, &w, &h);
        ASSERT_EQ(sprite.sourceRect.w, w);
        ASSERT_EQ(sprite.sourceRect.h, h);
    }
}

TEST_F(EntityFactoryTest, CreateCharacterInvalidAlias) {
    entt::entity player = factory.CreateCharacter("nonexistent_player_alias");
    ASSERT_EQ(player, entt::null);
}

TEST_F(EntityFactoryTest, CreateItemValid) {
    Vector2D initialPos = {50.0f, 75.0f};
    entt::entity item = factory.CreateItem("test_item", initialPos);

    ASSERT_NE(item, entt::null);
    ASSERT_TRUE(registry.valid(item));

    // Check TransformComponent (always added)
    ASSERT_TRUE(registry.all_of<TransformComponent>(item));
    const auto& transform = registry.get<TransformComponent>(item);
    ASSERT_FLOAT_EQ(transform.position.x, initialPos.x);
    ASSERT_FLOAT_EQ(transform.position.y, initialPos.y);
    // Default scale for items if not specified in definition or ApplyComponentData
    ASSERT_FLOAT_EQ(transform.scale.x, 1.0f); 
    ASSERT_FLOAT_EQ(transform.scale.y, 1.0f);

    // Item definition currently has no componentsToAdd, so no further checks here.
    // If components were added, they should be checked like in CreateCharacterValid.

    // Check SpriteComponent for item
    ASSERT_TRUE(registry.all_of<SpriteComponent>(item));
    const auto& itemSprite = registry.get<SpriteComponent>(item);
    ASSERT_NE(itemSprite.texture, nullptr); // Should have a texture if "item_tex_alias" was loaded
    if (itemSprite.texture == nullptr) {
        GTEST_LOG_(INFO) << "SpriteComponent.texture is null in CreateItemValid, dummy_texture.png likely not loaded for item_tex_alias.";
    } else {
        int w, h;
        SDL_QueryTexture(itemSprite.texture, nullptr, nullptr, &w, &h);
        ASSERT_EQ(itemSprite.sourceRect.w, w);
        ASSERT_EQ(itemSprite.sourceRect.h, h);
    }
}

TEST_F(EntityFactoryTest, CreateItemInvalidAlias) {
    entt::entity item = factory.CreateItem("nonexistent_item_alias");
    ASSERT_EQ(item, entt::null);
}

// Tests for ResourceManager loading definitions (subset, as RM has its own test file)
TEST_F(ResourceManagerTest, LoadCharacterDefinitionFromFile) {
    // This test is more about RM, but uses the fixture.
    // test_player_def.json is loaded in EntityFactoryTest::SetUp
    Content::CharacterDefinition* def = rm.GetCharacterDefinition("test_player");
    ASSERT_NE(def, nullptr);
    if (def) {
        ASSERT_EQ(def->id, "test_player");
        ASSERT_EQ(def->friendlyName, "Test Player");
    }
}

TEST_F(ResourceManagerTest, LoadItemDefinitionFromFile) {
    // test_item_def.json is loaded in EntityFactoryTest::SetUp
    Content::ItemDefinition* def = rm.GetItemDefinition("test_item");
    ASSERT_NE(def, nullptr);
    if (def) {
        ASSERT_EQ(def->id, "test_item");
        ASSERT_EQ(def->friendlyName, "Test Item");
    }
}
