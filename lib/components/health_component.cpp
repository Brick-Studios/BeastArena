#include "components/health_component.hpp"

HealthComponent::HealthComponent(double max_health, EntityFunction on_death, std::optional<EntityFunction> revive, std::optional<int> points_on_kill)
    : max_health(max_health), health(max_health), on_death(on_death), points_on_kill(points_on_kill), revive(revive) {}

std::string HealthComponent::getNameStatic() {
    return "HealthComponent";
}