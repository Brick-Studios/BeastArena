#ifndef FILE_GAME_SYSTEM
#define FILE_GAME_SYSTEM

#include <memory>
#include "entities/entity_factory.hpp"
#include "brickengine/systems/system.hpp"
#include "controllers/game_controller.hpp"

#include "brickengine/components/player_component.hpp"
#include "components/health_component.hpp"

class GameSystem : public System {
public:
    GameSystem(std::shared_ptr<EntityManager> em, GameController& gc);
    void update(double deltatime) override;
    void reset() override;
private:
    std::shared_ptr<EntityManager> entity_manager;
    GameController& game_controller;
    std::set<int> dead_players;
    double timer = 0;
    int seconds = 3;
};

#endif // FILE_GAME_SYSTEM