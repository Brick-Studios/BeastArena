#ifndef FILE_PICKUP_SYSTEM_HPP
#define FILE_PICKUP_SYSTEM_HPP

#include "systems/beast_system.hpp"
#include "brickengine/collision_detector.hpp"

class PickupSystem : public BeastSystem {
public:
    PickupSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef);
    void update(double delta_time);
    ~PickupSystem() = default;
private:
    std::shared_ptr<CollisionDetector> collision_detector;
};

#endif // FILE_PICKUP_SYSTEM_HPP