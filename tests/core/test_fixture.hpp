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

    // Mock Renderer for testing rendering calls
    // Needs to be defined within the class or as a separate class
    // For simplicity, defining it here if it's only for SDLTestFixture derivatives.
    // If used more broadly, move to its own header or a general test utility header.
    class TestMockRenderer : public Renderer {
    public:
        TestMockRenderer() : Renderer(nullptr) {} // Base class constructor

        struct DrawTextureCall {
            SDL_Texture* texture;
            SDL_Rect srcRect;
            SDL_Rect dstRect;
            double angle;
            SDL_Point center; // Store actual point, not pointer
            SDL_RendererFlip flip;
            SDL_Color colorTint;
        };
        std::vector<DrawTextureCall> drawTextureCalls;
        int drawRectCallCount = 0; // From existing mock in systems_test

        void DrawTexture(SDL_Texture* texture, 
                         const SDL_Rect& srcRect, 
                         const SDL_Rect& dstRect, 
                         double angle, 
                         const SDL_Point* center_ptr, 
                         SDL_RendererFlip flip,
                         const SDL_Color& colorTint) override {
            SDL_Point center_val = {0,0};
            if(center_ptr) center_val = *center_ptr;
            drawTextureCalls.push_back({texture, srcRect, dstRect, angle, center_val, flip, colorTint});
        }

        void DrawRect(int x, int y, int w, int h, bool fill) override {
            drawRectCallCount++;
        }
        void SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) override {
            // Do nothing or record if needed
        }
        void Clear() override {
            // Do nothing or record
        }
        void Present() override {
            // Do nothing or record
        }
    };
};


#endif // TEST_FIXTURE_HPP
