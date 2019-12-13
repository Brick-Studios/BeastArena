#include "components/name_selection_component.hpp"

NameSelectionComponent::NameSelectionComponent(int player_id, int left_arrow_entity_id, int right_arrow_entity_id) : player_id(player_id), selected_name(""), is_picking(false), picked(false), left_arrow_entity_id(left_arrow_entity_id), right_arrow_entity_id(right_arrow_entity_id) {};

std::string NameSelectionComponent::NameSelectionComponent::getNameStatic() {
    return "NameSelectionComponent";
}