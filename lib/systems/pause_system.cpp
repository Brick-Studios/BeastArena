#include "systems/pause_system.hpp"

#include "brickengine/input.hpp"
#include "player_input.hpp"
#include "brickengine/components/player_component.hpp"

PauseSystem::PauseSystem(std::shared_ptr<EntityManager> em, GameController& game_controller)
    :  System(em), em(em), game_controller(game_controller) {}

void PauseSystem::update(double){
    auto& input = BrickInput<PlayerInput>::getInstance();
    auto player_entities = em->getEntitiesByComponent<PlayerComponent>();
    
    for(auto& [entity_id, player] : player_entities) {
        if(input.checkInput(player->player_id, PlayerInput::PAUSE)) {
            game_controller.pauseGame();

            // Break so only 1 player can pause per tick
            break;
        }
    }
}