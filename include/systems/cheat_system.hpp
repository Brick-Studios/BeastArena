#ifndef FILE_CHEAT_SYSTEM_HPP
#define FILE_CHEAT_SYSTEM_HPP

#include "systems/beast_system.hpp"

#include "controllers/game_controller.hpp"

class CheatSystem : public BeastSystem {
public:
    CheatSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef, GameController& gc);
    void update(double delta_time);
private:
    GameController& game_controller;
};

#endif // FILE_CHEAT_SYSTEM_HPP