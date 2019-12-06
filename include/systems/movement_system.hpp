#ifndef FILE_MOVEMENT_SYSTEM_HPP
#define FILE_MOVEMENT_SYSTEM_HPP

#include <memory>
#include "brickengine/collision_detector_2.hpp"
#include "systems/beast_system.hpp"
#include "entities/entity_factory.hpp"

class MovementSystem : public BeastSystem {
public:
    MovementSystem(CollisionDetector2& cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef);
    void update(double deltatime);
private:
    static constexpr double JUMP_FORCE = 100'000;
    static constexpr double MOVEMENT_FORCE = 70'000;

    CollisionDetector2& collision_detector;
};

#endif // FILE_MOVEMENT_SYSTEM_HPP
