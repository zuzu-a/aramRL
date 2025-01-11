#include <rogue.hpp>

class EntityManager {
public:
    Entity createEntity() {
        return nextEntity++;
    }

    template <typename T>
    void addComponent(Entity entity, T component) {
        auto& componentArray = getComponentArray<T>();
        componentArray[entity] = std::move(component);
    }

    template <typename T>
    T* getComponent(Entity entity) {
        auto& componentArray = getComponentArray<T>();
        if (componentArray.count(entity)) {
            return &componentArray[entity];
        }
        return nullptr;
    }

private:
    Entity nextEntity = 0;
    std::unordered_map<std::type_index, std::any> components;

    template <typename T>
    std::unordered_map<Entity, T>& getComponentArray() {
        auto typeIndex = std::type_index(typeid(T));
        if (!components.count(typeIndex)) {
            components[typeIndex] = std::unordered_map<Entity, T>();
        }
        return *std::any_cast<std::unordered_map<Entity, T>>(&components[typeIndex]);
    }
};