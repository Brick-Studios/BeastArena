#include "components/damage_component.hpp"

DamageComponent::DamageComponent(double damage) : damage(damage) {}

std::string DamageComponent::DamageComponent::getNameStatic() {
    return "DamageComponent";
}