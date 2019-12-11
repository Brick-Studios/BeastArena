#ifndef FILE_PAUSE_SYSTEM_HPP
#define FILE_PAUSE_SYSTEM_HPP

#include "brickengine/systems/system.hpp"
#include "controllers/game_controller.hpp"

class PauseSystem : public System {
public:
    PauseSystem(std::shared_ptr<EntityManager> em, GameController& game_controller);
    void update(double delta_time);
private:
    std::shared_ptr<EntityManager> em;
    GameController& game_controller;
};

#endif // FILE_PAUSE_SYSTEM_HPP