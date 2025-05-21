#include <gtest/gtest.h>
#include <SDL.h>
#include <SDL_image.h> // If IMG_Init is tested

TEST(SDLInitializationTest, CanInitializeSDLAndWindow) {
    ASSERT_EQ(SDL_Init(SDL_INIT_VIDEO), 0); // Check SDL_Init succeeds

    // Attempt to create a window (even if not shown)
    SDL_Window* window = SDL_CreateWindow(
        "Test Window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        100, 100,
        SDL_WINDOW_HIDDEN // Hidden for test purposes
    );
    ASSERT_NE(window, nullptr); // Check window creation succeeds

    SDL_DestroyWindow(window);
    SDL_Quit();
}

TEST(SDLImageInitializationTest, CanInitializeSDLImage) {
    ASSERT_EQ(SDL_Init(SDL_INIT_VIDEO), 0); // SDL_image needs SDL_Init
    int flags = IMG_INIT_PNG | IMG_INIT_JPG;
    ASSERT_EQ(IMG_Init(flags) & flags, flags); // Check IMG_Init for specific formats
    IMG_Quit();
    SDL_Quit();
}
