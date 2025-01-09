char checkKeyPress(SDL_Event event) {
        if(event.key.keysym.sym) {
            switch (event.type) {
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