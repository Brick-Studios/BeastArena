#ifndef FILE_GAME_SPEED_SYSTEM_HPP
#define FILE_GAME_SPEED_SYSTEM_HPP

#include <memory>

#include "brickengine/systems/system.hpp"

class GameSpeedSystem : public System {
public:
    GameSpeedSystem(std::shared_ptr<EntityManager> em, double &delta_time_modifier);
    void update(double delta_time);
private:
    double &delta_time_modifier;
    const double step_size = 0.1;
};

#endif // FILE_GAME_SPEED_SYSTEM_HPP