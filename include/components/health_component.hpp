#ifndef FILE_HEALTH_COMPONENT_HPP
#define FILE_HEALTH_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"
#include <functional>
#include <optional>

class HealthComponent : public ComponentImpl<HealthComponent> {
public:
    using EntityFunction = std::function<void (int entity_id)>;
    HealthComponent(double max_health, EntityFunction on_death, std::optional<EntityFunction> revive = std::nullopt, std::optional<int> points_on_kill = std::nullopt);
    static std::string getNameStatic();

    // Data
    double health;
    double max_health;
    std::optional<int> points_on_kill;
    EntityFunction on_death;
    std::optional<EntityFunction> revive;
};

#endif // FILE_HEALTH_COMPONENT_HPP