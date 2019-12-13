#include "brickengine/input.hpp"

#include "systems/ui_system.hpp"
#include "player_input.hpp"

UISystem::UISystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef, GameController& gc)
    : game_controller(gc), BeastSystem(ef, em) {}

void UISystem::update(double deltatime) {
    auto& input = BrickInput<PlayerInput>::getInstance();
    for(int i = 1; i <= 4; ++i)
        if(input.checkInput(i, PlayerInput::QUICK_PLAY))
            game_controller.loadLobby();
}