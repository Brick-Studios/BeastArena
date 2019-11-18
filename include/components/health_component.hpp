#ifndef FILE_HEALTH_COMPONENT_HPP
#define FILE_HEALTH_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"
#include <functional>

class HealthComponent : public ComponentImpl<HealthComponent> {
public:
    using DeathFunction = std::function<void (int entity_id)>;
    HealthComponent(double health, DeathFunction on_death);
    static std::string getNameStatic();

    // Data
    double health;
    DeathFunction on_death;
};

#endif // FILE_HEALTH_COMPONENT_HPP