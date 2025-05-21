#ifndef PLAYER_COMPONENT_HPP
#define PLAYER_COMPONENT_HPP

struct PlayerComponent {
    bool is_active = true;
    int player_id = 0;
    // Could add more player-specific data like health, score, etc.

    PlayerComponent() = default;
    explicit PlayerComponent(int id, bool active = true) : player_id(id), is_active(active) {}
};

#endif // PLAYER_COMPONENT_HPP
