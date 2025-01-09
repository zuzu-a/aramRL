#include <rogue.hpp>

int main(int argc, char* argv[]) {
    int playerx=40, playery=25;

    auto console = tcod::Console(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Context configuration
    auto params = TCOD_ContextParams{};
        params.tcod_version = TCOD_COMPILEDVERSION;
        params.console      = console.get();
        params.window_title = "Aram";
        params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
        params.argc = argc;
        params.argv = argv;

    auto context = tcod::Context(params);

    while (1) {
        TCOD_console_clear(console.get());
        tcod::print(console, {0, 0}, "Hello World", std::nullopt, std::nullopt);
        context.present(console);

        SDL_Event event;
        SDL_WaitEvent(nullptr);

        while(SDL_PollEvent(&event)) {
            context.convert_event_coordinates(event);

            switch (event.type) {
                case SDL_QUIT:
                    return 0; // Exit game.

                case SDL_KEYUP:
                    switch(event.key.keysym.sym) {
                        case SDLK_w:
                            playery--;
                        case SDLK_s:
                            playery++;
                        case SDLK_a:
                            playerx++;
                        case SDLK_d:
                            playerx--;
                    }
            }
        }
    }
}