#ifndef FILE_DAMAGE_SYSTEM_HPP
#define FILE_DAMAGE_SYSTEM_HPP

#include "systems/beast_system.hpp"
#include "brickengine/collision_detector.hpp"
#include "components/damage_component.hpp"

class DamageSystem : public BeastSystem {
public:
    DamageSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef);
    void update(double delta_time);
    void collide(DamageComponent* damage_comp, CollisionReturnValues collision);
private:
    std::shared_ptr<CollisionDetector> collision_detector;
};

#endif // FILE_DAMAGE_SYSTEM_HPP