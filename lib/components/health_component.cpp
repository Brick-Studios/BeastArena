#include "components/health_component.hpp"

HealthComponent::HealthComponent(double health, DeathFunction on_death) : health(health), on_death(on_death) {}

std::string HealthComponent::HealthComponent::getNameStatic() {
    return "HealthComponent";
}