#ifndef FILE_PICKUP_SYSTEM_HPP
#define FILE_PICKUP_SYSTEM_HPP

#include "systems/beast_system.hpp"
#include "brickengine/collision_detector_2.hpp"

class PickupSystem : public BeastSystem {
public:
    PickupSystem(CollisionDetector2& cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef);
    void update(double delta_time);
private:
    CollisionDetector2& collision_detector;
};

#endif // FILE_PICKUP_SYSTEM_HPP