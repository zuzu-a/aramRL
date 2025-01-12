#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <SDL_events.h>

class Events {
public:
    static const    SDL_Event   &Event()            noexcept { return m_event; }
    static const    uint8_t     *KeyboardState()    noexcept { return SDL_GetKeyboardState(NULL); }

private:
    static inline   SDL_Event   m_event{};
};

#endif