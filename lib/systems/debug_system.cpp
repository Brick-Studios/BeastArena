#include <functional>

#include "player_input.hpp"
#include "systems/debug_system.hpp"
#include "brickengine/input.hpp"

DebugSystem::DebugSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef,
                        std::function<void()> on_reload)
    : BeastSystem(ef, em), on_reload(on_reload) {}

void DebugSystem::update(double) {
    auto& input = BrickInput<PlayerInput>::getInstance();

    // User wants to reload the level to view their newly made changes.
    if (input.checkInput(1, PlayerInput::REFRESH)){
        this->on_reload();
    }
}