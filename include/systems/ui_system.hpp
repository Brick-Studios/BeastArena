#ifndef FILE_UI_SYSTEM_HPP
#define FILE_UI_SYSTEM_HPP

#include "systems/beast_system.hpp"

#include "controllers/game_controller.hpp"

class UISystem : public BeastSystem {
public:
    UISystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef, GameController& gc);
    void update(double delta_time);
private:
    GameController& game_controller;
};

#endif // FILE_UI_RESPAWN_SYSTEM_HPP 