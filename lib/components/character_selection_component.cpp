#include "components/character_selection_component.hpp"

CharacterSelectionComponent::CharacterSelectionComponent(int player_id) : player_id(player_id), joined(false), selected_character(Character::GORILLA), picked(false), player_entity_id(-1) {};

std::string CharacterSelectionComponent::CharacterSelectionComponent::getNameStatic() {
    return "CharacterSelectionComponent";
}