#include <gtest/gtest.h>
#include "inc/core/content/CharacterDefinition.hpp"
#include "inc/core/content/ItemDefinition.hpp"
#include "inc/core/content/TilesetDefinition.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// CharacterDefinition Tests
TEST(ContentDefinitionTest, CharacterFromJsonValid) {
    json j = R"({
        "id": "char_test",
        "friendlyName": "Test Character",
        "textureAlias": "char_tex",
        "speed": 120.5,
        "health": 80,
        "initialScale": {"x": 1.2, "y": 1.2},
        "componentsToAdd": [
            {"name": "TestComponent", "properties": {"value": 10}}
        ]
    })"_json;

    Content::CharacterDefinition def;
    ASSERT_TRUE(Content::CharacterDefinition::FromJson(j, def));
    ASSERT_EQ(def.id, "char_test");
    ASSERT_EQ(def.friendlyName, "Test Character");
    ASSERT_EQ(def.textureAlias, "char_tex");
    ASSERT_FLOAT_EQ(def.speed, 120.5f);
    ASSERT_EQ(def.health, 80);
    ASSERT_FLOAT_EQ(def.initialScale.x, 1.2f);
    ASSERT_FLOAT_EQ(def.initialScale.y, 1.2f);
    ASSERT_EQ(def.componentsToAdd.size(), 1);
    ASSERT_EQ(def.componentsToAdd[0].name, "TestComponent");
    ASSERT_EQ(def.componentsToAdd[0].properties["value"], 10);
    ASSERT_EQ(def.type, Content::Type::CHARACTER_DEFINITION);
}

TEST(ContentDefinitionTest, CharacterFromJsonMissingRequired) {
    json j = R"({
        "friendlyName": "Missing ID Character"
        // "id" is missing
        // "textureAlias" is missing
    })"_json;
    Content::CharacterDefinition def;
    ASSERT_FALSE(Content::CharacterDefinition::FromJson(j, def));
}

TEST(ContentDefinitionTest, CharacterFromJsonDefaults) {
    json j = R"({
        "id": "char_defaults",
        "textureAlias": "char_tex_defaults"
        // friendlyName, speed, health, initialScale, componentsToAdd use defaults
    })"_json;
    Content::CharacterDefinition def;
    ASSERT_TRUE(Content::CharacterDefinition::FromJson(j, def));
    ASSERT_EQ(def.id, "char_defaults");
    ASSERT_EQ(def.friendlyName, "char_defaults"); // Defaults to id
    ASSERT_EQ(def.textureAlias, "char_tex_defaults");
    ASSERT_FLOAT_EQ(def.speed, 100.0f); // Default value
    ASSERT_EQ(def.health, 100);         // Default value
    ASSERT_FLOAT_EQ(def.initialScale.x, 1.0f); // Default value
    ASSERT_FLOAT_EQ(def.initialScale.y, 1.0f); // Default value
    ASSERT_TRUE(def.componentsToAdd.empty());
}

// ItemDefinition Tests
TEST(ContentDefinitionTest, ItemFromJsonValid) {
    json j = R"({
        "id": "item_test",
        "friendlyName": "Test Item",
        "description": "A test item.",
        "textureAlias": "item_tex",
        "componentsToAdd": [
            {"name": "EffectComponent", "properties": {"duration": 5.0}}
        ]
    })"_json;
    Content::ItemDefinition def;
    ASSERT_TRUE(Content::ItemDefinition::FromJson(j, def));
    ASSERT_EQ(def.id, "item_test");
    ASSERT_EQ(def.friendlyName, "Test Item");
    ASSERT_EQ(def.description, "A test item.");
    ASSERT_EQ(def.textureAlias, "item_tex");
    ASSERT_EQ(def.componentsToAdd.size(), 1);
    ASSERT_EQ(def.componentsToAdd[0].name, "EffectComponent");
    ASSERT_FLOAT_EQ(def.componentsToAdd[0].properties["duration"], 5.0f);
    ASSERT_EQ(def.type, Content::Type::ITEM_DEFINITION);
}

TEST(ContentDefinitionTest, ItemFromJsonMissingRequired) {
    json j = R"({
        "friendlyName": "Missing ID Item"
        // "id" is missing
        // "textureAlias" is missing
    })"_json;
    Content::ItemDefinition def;
    ASSERT_FALSE(Content::ItemDefinition::FromJson(j, def));
}

// TilesetDefinition Tests
TEST(ContentDefinitionTest, TilesetFromJsonValid) {
    json j = R"({
        "id": "tileset_def_test",
        "jsonPath": "path/to/tileset.json",
        "textureAlias": "tileset_tex_alias"
    })"_json;
    Content::TilesetDefinition def;
    ASSERT_TRUE(Content::TilesetDefinition::FromJson(j, def));
    ASSERT_EQ(def.id, "tileset_def_test");
    ASSERT_EQ(def.jsonPath, "path/to/tileset.json");
    ASSERT_EQ(def.textureAlias, "tileset_tex_alias");
    ASSERT_EQ(def.type, Content::Type::TILESET_DEFINITION);
}

TEST(ContentDefinitionTest, TilesetFromJsonMissingRequired) {
    json j = R"({
        "id": "missing_path_tileset"
        // "jsonPath" is missing
        // "textureAlias" is missing
    })"_json;
    Content::TilesetDefinition def;
    ASSERT_FALSE(Content::TilesetDefinition::FromJson(j, def));
}
