#ifndef FILE_WANDERING_SYSTEM_HPP
#define FILE_WANDERING_SYSTEM_HPP

#include <memory>
#include "brickengine/collision_detector.hpp"
#include "systems/beast_system.hpp"

class CritterSystem : public BeastSystem {
public:
    CritterSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef);
    void init();
    void update(double deltatime);
private:
    static constexpr double TERMINAL_VELOCITY = 7'000;
    static constexpr double JUMP_FORCE = 16'000;
    static constexpr double MOVEMENT_FORCE = 0.5;
    std::shared_ptr<CollisionDetector> collision_detector;
};

#endif // FILE_WANDERING_SYSTEM_HPP