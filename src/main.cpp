#include <rogue.hpp>

int main(int argc, char* argv[]) {

    int playerx=40, playery=25;

    // TILESET CONFIGURATION
    
    
    auto default_tileset = tcod::load_tilesheet("tileset/Terrain.png", {16, 110}, tcod::CHARMAP_CP437);
    default_tileset.get() -> character_map_length = 175;
    auto console = tcod::Console(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Context configuration
    auto params = TCOD_ContextParams{};
        params.tcod_version = TCOD_COMPILEDVERSION;
        params.console      = console.get();
        params.window_title = "Aram";
        params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
        params.argc = argc;
        params.argv = argv;
        params.tileset = default_tileset.get();

    auto context = tcod::Context(params);

    while (1) {
        TCOD_console_clear(console.get());

        static constexpr std::array<int, 9> LEGEND = {'+', '-', '+', '|', '32', '|', '+', '-', '+'};
        tcod::draw_frame(console, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, LEGEND, {{255, 255, 255}}, {{0, 0, 0}});

        tcod::print(console, {SCREEN_WIDTH/2, 0}, "ARAM", std::nullopt, std::nullopt);
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