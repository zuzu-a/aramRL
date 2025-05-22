#include "inc/core/systems/RenderSystem.hpp" // Adjust path
#include "inc/core/components/TransformComponent.hpp" // Adjust path
#include "inc/core/components/SpriteComponent.hpp"   // Adjust path
#include "inc/core/render.hpp" // For Renderer class
#include <vector>       // For sorting by z_order
#include <algorithm>    // For std::sort

RenderSystem::RenderSystem() {}
RenderSystem::~RenderSystem() {}

struct RenderableEntity {
    entt::entity entity;
    const TransformComponent* transform;
    const SpriteComponent* sprite;
};

// Custom comparison function for sorting RenderableEntity objects
bool CompareRenderables(const RenderableEntity& a, const RenderableEntity& b) {
    if (a.sprite->z_order != b.sprite->z_order) {
        return a.sprite->z_order < b.sprite->z_order;
    }
    // Optional: Further sort by Y position for pseudo-3D effect if z_orders are equal
    // This can be important if z_orders are the same but visual layering based on Y is desired.
    return a.transform->position.y < b.transform->position.y; 
}

void RenderSystem::Update(entt::registry& registry, Renderer& renderer, float deltaTime /* for interpolation later */) {
    auto view = registry.view<const TransformComponent, const SpriteComponent>();
    
    // Collect renderable entities for sorting
    std::vector<RenderableEntity> renderables;
    renderables.reserve(view.size_hint()); // Pre-allocate memory

    for (auto entity : view) {
        const auto& transform = view.get<const TransformComponent>(entity);
        const auto& sprite = view.get<const SpriteComponent>(entity);

        if (!sprite.visible || !sprite.texture) {
            continue; // Skip invisible or textureless sprites
        }
        renderables.push_back({entity, &transform, &sprite});
    }

    // Sort entities by z_order, then by Y
    std::sort(renderables.begin(), renderables.end(), CompareRenderables);

    for (const auto& renderable : renderables) {
        const auto& transform = *renderable.transform;
        const auto& sprite = *renderable.sprite;

        SDL_Rect destRect;
        // For now, assume sourceRect width/height ARE the desired render dimensions scaled by transform.scale
        // Later, SpriteComponent could have explicit width/height fields.
        destRect.w = static_cast<int>(sprite.sourceRect.w * transform.scale.x);
        destRect.h = static_cast<int>(sprite.sourceRect.h * transform.scale.y);
        
        // Calculate top-left position considering the pivot
        destRect.x = static_cast<int>(transform.position.x - (destRect.w * sprite.pivot.x));
        destRect.y = static_cast<int>(transform.position.y - (destRect.h * sprite.pivot.y));
        
        // The pivot point for SDL_RenderCopyEx is relative to the destRect's top-left corner
        SDL_Point pivotPoint = { 
            static_cast<int>(destRect.w * sprite.pivot.x), 
            static_cast<int>(destRect.h * sprite.pivot.y) 
        };

        renderer.DrawTexture(sprite.texture, sprite.sourceRect, destRect, 
                             transform.rotation, &pivotPoint, sprite.flip, sprite.colorTint);
    }
}
