#include "inc/core/render.hpp" // Or "render.hpp" if paths are set up
#include <iostream> // For placeholder messages

Renderer::Renderer(SDL_Renderer* sdlRenderer) : m_sdlRenderer(sdlRenderer) {
    if (!m_sdlRenderer) {
        // Handle error - though main.cpp should ensure it's valid
        // For example, throw an exception or set an internal error state
        std::cerr << "Error: SDL_Renderer passed to Renderer constructor is null!" << std::endl;
    }
}

Renderer::~Renderer() {
    // Renderer doesn't own m_sdlRenderer, so no SDL_DestroyRenderer here.
    // The SDL_Renderer is managed (created and destroyed) in main.cpp.
}

void Renderer::Clear() {
    if (!m_sdlRenderer) return;
    SDL_RenderClear(m_sdlRenderer);
}

void Renderer::Present() {
    if (!m_sdlRenderer) return;
    SDL_RenderPresent(m_sdlRenderer);
}

void Renderer::SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    if (!m_sdlRenderer) return;
    SDL_SetRenderDrawColor(m_sdlRenderer, r, g, b, a);
}

void Renderer::DrawRect(int x, int y, int w, int h, bool fill) {
    if (!m_sdlRenderer) return;
    SDL_Rect rect = {x, y, w, h};
    if (fill) {
        SDL_RenderFillRect(m_sdlRenderer, &rect);
    } else {
        SDL_RenderDrawRect(m_sdlRenderer, &rect);
    }
}
 
void Renderer::DrawChar(char ch, int x, int y /* SDL_Color color, Font* font */) {
    if (!m_sdlRenderer) return;
    // Placeholder: Draw a small white rectangle for the character
    std::cout << "Renderer::DrawChar called for '" << ch << "' at (" << x << "," << y << "). Font rendering TODO." << std::endl;
    // Get current color to restore it later, or just set it to white for the placeholder
    SDL_Color restoreColor = {0,0,0,0};
    SDL_GetRenderDrawColor(m_sdlRenderer, &restoreColor.r, &restoreColor.g, &restoreColor.b, &restoreColor.a);
    
    SDL_SetRenderDrawColor(m_sdlRenderer, 255, 255, 255, 255); // White for the placeholder char
    SDL_Rect char_rect = {x, y, 8, 12}; // Example size for a char
    SDL_RenderFillRect(m_sdlRenderer, &char_rect);

    SDL_SetRenderDrawColor(m_sdlRenderer, restoreColor.r, restoreColor.g, restoreColor.b, restoreColor.a); // Restore color
}
