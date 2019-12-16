#include "systems/lobby_system.hpp"

#include <algorithm>

#include "brickengine/input.hpp"
#include "player_input.hpp"
#include "components/character_selection_component.hpp"
#include "components/name_selection_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"
#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/json/json.hpp"

LobbySystem::LobbySystem(std::shared_ptr<EntityFactory> ef, std::shared_ptr<EntityManager> em, GameController& gc)
    : game_controller(gc), BeastSystem(ef, em), ef(ef), em(em) { };

void LobbySystem::update(double) {
    auto& input = BrickInput<PlayerInput>::getInstance();

    auto character_selector_entities = em->getEntitiesByComponent<CharacterSelectionComponent>();

    for(auto& [character_selector_entity_id, character_selector] : character_selector_entities) {
        auto children = em->getChildrenWithComponent<NameSelectionComponent>(character_selector_entity_id);
        if(children.size() > 0) {
            auto& name_selection = children.at(0);

            if(input.checkInput(character_selector->player_id, PlayerInput::GRAB)) {
                if(!character_selector->joined) {
                    // Player wants to play
                    character_selector->joined = true;
                    for(auto& character : picked_characters) {
                        // Select the first available character
                        if(!character.second) {
                            character_selector->selected_character = character.first;
                            ef->changeCharacterSelectorTexture(character_selector_entity_id, character.first, true);

                            auto texture_component_left_arrow_character = em->getComponent<TextureComponent>(character_selector->left_arrow_entity_id);
                            texture_component_left_arrow_character->getTexture()->alpha = 255;

                            auto texture_component_right_arrow_character = em->getComponent<TextureComponent>(character_selector->right_arrow_entity_id);
                            texture_component_right_arrow_character->getTexture()->alpha = 255;

                            break;
                        }
                    }
                } else {
                    // Player selected character
                    if(!character_selector->picked) {
                        // Check if character is available
                        bool available = true;
                        for(auto character : picked_characters) {
                            if(character.first == character_selector->selected_character && character.second) {
                                available = false;
                                break;
                            }
                        }

                        if(available) {
                            if(character_selector->selected_character != Character::RANDOM) {
                                for(auto& character : picked_characters) {
                                    if(character.first == character_selector->selected_character) {
                                        character.second = true;
                                        break;
                                    }
                                }

                                // Update the other selector so they do not show the character chosen by this player
                                auto to_update_selector_entities = em->getEntitiesByComponent<CharacterSelectionComponent>();
                                for(auto& [to_update_entity_id, to_update_selector] : to_update_selector_entities) {
                                    if(character_selector_entity_id != to_update_entity_id && to_update_selector->joined && !to_update_selector->picked && to_update_selector->selected_character == character_selector->selected_character) {
                                        // This is not the current selector, the player has joined but not picked a character and the character currently displayed is just picked by the other player
                                        for(auto& character : picked_characters) {
                                            // Select the first available character
                                            if(!character.second) {
                                                to_update_selector->selected_character = character.first;
                                                ef->changeCharacterSelectorTexture(to_update_entity_id, character.first, true);
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            character_selector->picked = true;

                            auto transform_component = em->getComponent<TransformComponent>(character_selector_entity_id);

                            auto texture_component_left_arrow_character = em->getComponent<TextureComponent>(character_selector->left_arrow_entity_id);
                            texture_component_left_arrow_character->getTexture()->alpha = 0;

                            auto texture_component_right_arrow_character = em->getComponent<TextureComponent>(character_selector->right_arrow_entity_id);
                            texture_component_right_arrow_character->getTexture()->alpha = 0;

                            // When creating the checkmark, the relative modifier has already been applied so we use modifier 1 to prevent weird sizes and coordinates
                            ef->addToEntityManager(ef->createImage("menu/check.png", transform_component->x_pos, transform_component->y_pos, 84, 66, 1, Layers::Foreground, 255));

                            // Set the first available name
                            for(auto& name : picked_names) {
                                if(!name.second) {
                                    ef->changeNameSelectorName(name_selection.first, name.first, true, false);
                                    name_selection.second->selected_name = name.first;
                                    name_selection.second->is_picking = true;

                                    auto texture_component_left_arrow_name = em->getComponent<TextureComponent>(name_selection.second->left_arrow_entity_id);
                                    texture_component_left_arrow_name->getTexture()->alpha = 255;

                                    auto texture_component_right_arrow_name = em->getComponent<TextureComponent>(name_selection.second->right_arrow_entity_id);
                                    texture_component_right_arrow_name->getTexture()->alpha = 255;
                                    break;  
                                }
                            }
                        }
                    } else {
                        // The player already picked a character
                        if(name_selection.second->is_picking && !name_selection.second->picked) {
                            // Check if the name is available
                            for(auto& name : picked_names) {
                                if(name.first == name_selection.second->selected_name) {
                                    if(!name.second){
                                        name.second = true;
                                        name_selection.second->picked = true;
                                        ef->changeNameSelectorName(name_selection.first, name.first, true, true);

                                        // Update the other selector so they do not show the character chosen by this player
                                        auto to_update_name_selector_entities = em->getEntitiesByComponent<NameSelectionComponent>();
                                        for(auto& [to_update_entity_id, to_update_selector] : to_update_name_selector_entities) {
                                            if(character_selector_entity_id != to_update_entity_id && to_update_selector->is_picking && !to_update_selector->picked && to_update_selector->selected_name == name.first) {
                                                // This is not the current name selector, the player is picking a name but has not made a choise and the name currently displayed is just picked by the other player
                                                for(auto& to_update_name : picked_names) {
                                                    // Select the first available name
                                                    if(!to_update_name.second) {
                                                        to_update_selector->selected_name = to_update_name.first;
                                                        ef->changeNameSelectorName(to_update_entity_id, to_update_name.first, false, false);
                                                        break;
                                                    }
                                                }
                                            }
                                        }

                                        auto transform_component = em->getComponent<TransformComponent>(character_selector_entity_id);
                                        int id = ef->addToEntityManager(ef->createPlayer(character_selector->player_id, character_selector->selected_character, name.first, transform_component->x_pos, transform_component->y_pos));
                                        character_selector->player_entity_id = id;

                                        auto texture_component_left_arrow_name = em->getComponent<TextureComponent>(name_selection.second->left_arrow_entity_id);
                                        texture_component_left_arrow_name->getTexture()->alpha = 0;

                                        auto texture_component_right_arrow_name = em->getComponent<TextureComponent>(name_selection.second->right_arrow_entity_id);
                                        texture_component_right_arrow_name->getTexture()->alpha = 0;

                                        input.removeTimeToWait(character_selector->player_id, PlayerInput::X_AXIS);
                                        break;
                                    }
                                }
                            }
                        }
                        continue;
                    }
                }
            }

            if(character_selector->joined && !character_selector->picked) {
                bool changed_character = false; // This bool is to check if the character has been changed
                int character_change_amount = 1; // This int is needed when looking for the first available character

                auto x_movement = input.checkInput(character_selector->player_id, PlayerInput::X_AXIS);
                if (x_movement < 0) {
                    // Left
                    for(auto character : picked_characters) {
                        if(character.first == character_selector->selected_character) {
                            while(!changed_character) {
                                auto iterator = std::find(picked_characters.begin(), picked_characters.end(), character);
                                int index = std::distance(picked_characters.begin(), iterator);

                                int new_index = 0;
                                if(index - character_change_amount < 0) {
                                    new_index = picked_characters.size() - ((index - character_change_amount) * -1);
                                } else {
                                    new_index = index - character_change_amount;
                                }

                                if(picked_characters.at(new_index).second) {
                                    // This character is already picked
                                    character_change_amount++;
                                } else {
                                    character_selector->selected_character = picked_characters.at(new_index).first;
                                    changed_character = true;
                                }
                            }
                            break;
                        }
                    }
                } else if (x_movement > 0) {
                    // Right
                    for(auto character : picked_characters) {
                        if(character.first == character_selector->selected_character) {
                            while(!changed_character) {
                                auto iterator = std::find(picked_characters.begin(), picked_characters.end(), character);
                                int index = std::distance(picked_characters.begin(), iterator);

                                int new_index = 0;
                                if(index + character_change_amount > picked_characters.size() - 1) {
                                    new_index = 0 + ((index + character_change_amount) - picked_characters.size());
                                } else {
                                    new_index = index + character_change_amount;
                                }

                                if(picked_characters.at(new_index).second) {
                                    // This character is already picked
                                    character_change_amount++;
                                } else {
                                    character_selector->selected_character = picked_characters.at(new_index).first;
                                    changed_character = true;
                                }
                            }
                            break;
                        }
                    }
                }

                if(changed_character) {
                    entity_factory->changeCharacterSelectorTexture(character_selector_entity_id, character_selector->selected_character, false);
                }
            }

            if(character_selector->joined && character_selector->picked && name_selection.second->is_picking && !name_selection.second->picked){
                bool changed_name = false; // This bool is to check if the name has been changed
                int name_change_amount = 1; // This int is needed when looking for the first available name

                auto x_movement = input.checkInput(character_selector->player_id, PlayerInput::X_AXIS);
                if (x_movement < 0) {
                    // Left
                    for(auto name : picked_names) {
                        if(name.first == name_selection.second->selected_name) {
                            while(!changed_name) {
                                auto iterator = std::find(picked_names.begin(), picked_names.end(), name);
                                int index = std::distance(picked_names.begin(), iterator);

                                int new_index = 0;
                                if(index - name_change_amount < 0) {
                                    new_index = picked_names.size() - ((index - name_change_amount) * -1);
                                } else {
                                    new_index = index - name_change_amount;
                                }

                                if(picked_names.at(new_index).second) {
                                    // This name is already picked
                                    name_change_amount++;
                                } else {
                                    name_selection.second->selected_name = picked_names.at(new_index).first;
                                    changed_name = true;
                                }
                            }
                            break;
                        }
                    }
                } else if (x_movement > 0) {
                    // Right
                    for(auto name : picked_names) {
                        if(name.first == name_selection.second->selected_name) {
                            while(!changed_name) {
                                auto iterator = std::find(picked_names.begin(), picked_names.end(), name);
                                int index = std::distance(picked_names.begin(), iterator);

                                int new_index = 0;
                                if(index + name_change_amount > picked_names.size() - 1) {
                                    new_index = 0 + ((index + name_change_amount) - picked_names.size());
                                } else {
                                    new_index = index + name_change_amount;
                                }

                                if(picked_names.at(new_index).second) {
                                    // This name is already picked
                                    name_change_amount++;
                                } else {
                                    name_selection.second->selected_name = picked_names.at(new_index).first;
                                    changed_name = true;
                                }
                            }
                            break;
                        }
                    }
                }

                if(changed_name) {
                    entity_factory->changeNameSelectorName(name_selection.first, name_selection.second->selected_name, false, false);
                }
            }
        }
    }

    // Start tha game
    for(int i = 1; i <= 4; ++i)
        if(input.checkInput(1, PlayerInput::PAUSE))
            game_controller.startGame();
}

void LobbySystem::reset() {
    picked_characters = entity_factory->getPickedCharacters();

    picked_names.clear();
    Json names = Json("assets/names.json", true);
    for(std::string name : names.getStringVector("names")) {
        picked_names.push_back(std::pair<std::string, bool>(name, false));
    }
}
