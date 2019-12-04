#ifndef FILE_CHARACTER_SELECTION_COMPONENT_HPP
#define FILE_CHARACTER_SELECTION_COMPONENT_HPP

#include <optional>
#include <string>

#include "brickengine/components/component_impl.hpp"
#include "enums/character.hpp"

class CharacterSelectionComponent : public ComponentImpl<CharacterSelectionComponent> {
public:
    CharacterSelectionComponent(int player_id);
    static std::string getNameStatic();
    
    // Data
    int player_id;
    bool joined;
    
    Character selected_character;
    bool picked;
    int player_entity_id;
};

#endif // FILE_CHARACTER_SELECTION_COMPONENT_HPP