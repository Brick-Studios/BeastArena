#include "components/despawn_component.hpp"

DespawnComponent::DespawnComponent(bool despawn_on_collision, bool despawn_on_out_of_screen)
    : despawn_on_collision(despawn_on_collision), despawn_on_out_of_screen(despawn_on_out_of_screen) {}

std::string DespawnComponent::DespawnComponent::getNameStatic() {
    return "DespawnComponent";
}