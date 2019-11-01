#ifndef FILE_MOVEMENT_SYSTEM_HPP
#define FILE_MOVEMENT_SYSTEM_HPP

#include <memory>
#include "brickengine/collision_detector.hpp"
#include "systems/beast_system.hpp"
#include "entities/entity_factory.hpp"

class MovementSystem : public BeastSystem {
public:
    MovementSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef);
    ~MovementSystem() = default;
    void update(double deltatime);
private:
    static constexpr double TERMINAL_VELOCITY = 250'000;
    static constexpr double JUMP_FORCE = 50'000;
    static constexpr double MOVEMENT_FORCE = 0.1;

    std::shared_ptr<CollisionDetector> collisionDetector;
};

#endif // FILE_MOVEMENT_SYSTEM_HPP
