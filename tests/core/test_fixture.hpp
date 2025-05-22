#ifndef TEST_FIXTURE_HPP
#define TEST_FIXTURE_HPP

#include <gtest/gtest.h>
#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <filesystem> // For std::filesystem::create_directories
#include <iostream> // For std::cerr in fixture

namespace TestUtils {
    // Helper to create dummy files for testing
    inline void CreateDummyFile(const std::string& filePath, const std::string& content) {
        // Ensure directory exists
        std::filesystem::path p(filePath);
        if (p.has_parent_path()) {
            std::filesystem::create_directories(p.parent_path());
        }

        std::ofstream outFile(filePath);
        ASSERT_TRUE(outFile.is_open()) << "Failed to create dummy file: " << filePath;
        outFile << content;
        outFile.close();
    }
} // namespace TestUtils


class SDLTestFixture : public ::testing::Test {
public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

protected: // Changed back to protected as tests will derive from this
    static void SetUpTestSuite() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            FAIL() << "SDL_Init failed in SetUpTestSuite: " << SDL_GetError();
        }
        int imgFlags = IMG_INIT_PNG; // Only PNG needed for dummy textures
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            // This makes tests fail if SDL_image can't init, which is important for RM tests.
            FAIL() << "IMG_Init failed in SetUpTestSuite: " << IMG_GetError();
        }
    }

    static void TearDownTestSuite() {
        IMG_Quit();
        SDL_Quit();
    }

    void SetUp() override {
        window = SDL_CreateWindow("TestFixture", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 100, 100, SDL_WINDOW_HIDDEN);
        ASSERT_NE(window, nullptr) << "SDL_CreateWindow failed in SetUp: " << SDL_GetError();
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        ASSERT_NE(renderer, nullptr) << "SDL_CreateRenderer failed in SetUp: " << SDL_GetError();
        
        // Ensure test data directory exists
        std::filesystem::create_directories("tests/data");
    }

    void TearDown() override {
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        // SDL_Quit and IMG_Quit are handled in TearDownTestSuite
    }
};

#endif // TEST_FIXTURE_HPP
