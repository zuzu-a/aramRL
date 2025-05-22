#ifndef RENDER_HPP
#define RENDER_HPP

#include <SDL.h>

class Renderer {
public:
    Renderer(SDL_Renderer* sdlRenderer = nullptr); // Added default nullptr
    ~Renderer();

    SDL_Renderer* GetSDLRenderer() const { return m_sdlRenderer; } // Added getter

    void Clear();
    void Present();
    void SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    // Basic drawing functions
    void DrawRect(int x, int y, int w, int h, bool fill = false);
    void DrawChar(char ch, int x, int y /* SDL_Color color, Font* font */); // Placeholder for now
    void DrawTexture(SDL_Texture* texture, 
                     const SDL_Rect& srcRect, 
                     const SDL_Rect& dstRect, 
                     double angle, 
                     const SDL_Point* center, // Pivot point for rotation
                     SDL_RendererFlip flip,
                     const SDL_Color& colorTint);

private:
    SDL_Renderer* m_sdlRenderer;
    // TODO: Add Font loading/management (perhaps via ResourceManager) for DrawChar
};

#endif // RENDER_HPP
