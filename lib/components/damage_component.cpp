#include "components/damage_component.hpp"

DamageComponent::DamageComponent(double damage, bool despawn, std::optional<int> damage_dealer_entity_id)
    : damage(damage), despawn(despawn), damage_dealer_entity_id(damage_dealer_entity_id) {}

std::string DamageComponent::DamageComponent::getNameStatic() {
    return "DamageComponent";
}