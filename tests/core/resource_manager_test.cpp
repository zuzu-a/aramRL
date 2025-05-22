#include <gtest/gtest.h>
#include "inc/core/ResourceManager.hpp"
#include "inc/core/Tileset.hpp" // For checking Tileset properties
#include "test_fixture.hpp" // Include the shared test fixture

// Note: CreateDummyFile is now in TestUtils namespace in test_fixture.hpp

class ResourceManagerTest : public SDLTestFixture { // Inherit from shared fixture
protected:
    ResourceManager rm;

    void SetUp() override {
        SDLTestFixture::SetUp(); // Call base fixture's SetUp
        rm.Initialize(renderer); // Initialize ResourceManager with the renderer from the fixture
        
        // Ensure test data directory exists - already handled by SDLTestFixture::SetUp
    }
        
    void TearDown() override {
        // ResourceManager destructor handles its own cleanup
        SDLTestFixture::TearDown(); // Call base fixture's TearDown
    }
};

TEST_F(ResourceManagerTest, Initialize) {
    // Test if Initialize was called (m_renderer should not be null)
    // This is implicitly tested by SetUp, but an explicit check can be added
    // if RM had a getter for its renderer or an isInitialized flag.
    SUCCEED(); // If SetUp didn't fail, this is a basic pass.
}

TEST_F(ResourceManagerTest, LoadAndGetTexture) {
    // WORKER NOTE: This test requires "tests/data/dummy_texture.png" to be manually created.
    SDL_Texture* tex1 = rm.LoadTexture("tests/data/dummy_texture.png", "tex1");
    if (!tex1) {
        GTEST_SKIP() << "Skipping test: dummy_texture.png not found or load failed. Worker needs to create this file.";
    }
    ASSERT_NE(tex1, nullptr);

    SDL_Texture* retrievedTex1 = rm.GetTexture("tex1");
    ASSERT_EQ(tex1, retrievedTex1);
}

TEST_F(ResourceManagerTest, LoadTextureExistingAlias) {
    // WORKER NOTE: This test requires "tests/data/dummy_texture.png" to be manually created.
    SDL_Texture* tex1 = rm.LoadTexture("tests/data/dummy_texture.png", "tex_alias_test");
    if (!tex1) { GTEST_SKIP() << "Skipping test: dummy_texture.png not found for tex_alias_test."; }
       
    // Attempt to load another texture with the same alias.
    // WORKER NOTE: Requires "tests/data/another_dummy_texture.png" or behavior check against existing alias.
    // For simplicity, we'll test the "returns existing" behavior.
    SDL_Texture* tex2 = rm.LoadTexture("tests/data/dummy_texture.png", "tex_alias_test"); // Use same path for simplicity
    ASSERT_EQ(tex1, tex2); // Should return the first loaded texture for that alias
}


TEST_F(ResourceManagerTest, GetNonExistentTexture) {
    ASSERT_EQ(rm.GetTexture("nonexistent_alias"), nullptr);
}

TEST_F(ResourceManagerTest, LoadTextureMissingFile) {
    ASSERT_EQ(rm.LoadTexture("tests/data/nonexistent_texture.png", "bad_tex"), nullptr);
}

TEST_F(ResourceManagerTest, LoadAndGetTileset) {
    // WORKER NOTE: This test requires "tests/data/dummy_texture.png" to be manually created.
    SDL_Texture* sheetTex = rm.LoadTexture("tests/data/dummy_texture.png", "sheet_for_tileset");
    if (!sheetTex) { GTEST_SKIP() << "Skipping test: dummy_texture.png not found for sheet_for_tileset."; }

    std::string jsonContent = R"({
        "name": "TilesetFromRM", "tilewidth": 16, "tileheight": 16, "tilecount": 1, "columns": 1,
        "image": "dummy_texture.png" 
    })";
    TestUtils::CreateDummyFile("tests/data/rm_tileset.json", jsonContent);

    ASSERT_TRUE(rm.LoadTileset("tests/data/rm_tileset.json", "sheet_for_tileset", "my_tileset"));
       
    Tileset* tileset = rm.GetTileset("my_tileset");
    ASSERT_NE(tileset, nullptr);
    if (tileset) {
        ASSERT_EQ(tileset->getName(), "TilesetFromRM");
        ASSERT_EQ(tileset->getTexture(), sheetTex); // Verify it's using the loaded texture
    }
}
    
TEST_F(ResourceManagerTest, LoadTilesetMissingTextureAlias) {
    TestUtils::CreateDummyFile("tests/data/rm_tileset_no_tex_alias.json", R"({"name":"Test"})");
    ASSERT_FALSE(rm.LoadTileset("tests/data/rm_tileset_no_tex_alias.json", "nonexistent_tex_alias", "bad_tileset"));
}

TEST_F(ResourceManagerTest, GetNonExistentTileset) {
    ASSERT_EQ(rm.GetTileset("nonexistent_tileset_alias"), nullptr);
}
