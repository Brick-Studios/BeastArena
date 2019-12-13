#include "components/character_selection_component.hpp"

CharacterSelectionComponent::CharacterSelectionComponent(int player_id, int left_arrow_entity_id, int right_arrow_entity_id) : player_id(player_id), joined(false), selected_character(Character::GORILLA), picked(false), player_entity_id(-1), left_arrow_entity_id(left_arrow_entity_id), right_arrow_entity_id(right_arrow_entity_id) {};

std::string CharacterSelectionComponent::CharacterSelectionComponent::getNameStatic() {
    return "CharacterSelectionComponent";
}