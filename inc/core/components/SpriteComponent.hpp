#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include <SDL.h> // For SDL_Texture, SDL_Rect, SDL_Color, SDL_RendererFlip
#include "vector2d.hpp" // For pivot

struct SpriteComponent {
    SDL_Texture* texture = nullptr;      // Pointer to the texture (owned by ResourceManager)
    SDL_Rect sourceRect = {0,0,0,0};   // Portion of the texture to render
    SDL_Color colorTint = {255, 255, 255, 255}; // Tint color
    int z_order = 0;                    // For sorting (future use)
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    Vector2D pivot = {0.5f, 0.5f};      // Normalized pivot point (0,0 top-left, 0.5,0.5 center, 1,1 bottom-right)
    bool visible = true;

    SpriteComponent() = default;
    SpriteComponent(SDL_Texture* tex, const SDL_Rect& srcRect) 
        : texture(tex), sourceRect(srcRect) {}
};

#endif // SPRITE_COMPONENT_HPP
