#ifndef FILE_DAMAGE_SYSTEM_HPP
#define FILE_DAMAGE_SYSTEM_HPP

#include "systems/beast_system.hpp"
#include "brickengine/collision_detector_2.hpp"
#include "components/damage_component.hpp"

class DamageSystem : public BeastSystem {
public:
    DamageSystem(CollisionDetector2& cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef);
    void update(double delta_time);
    void collide(DamageComponent& damage_comp, const Collision& collision);
private:
    CollisionDetector2& collision_detector;
};

#endif // FILE_DAMAGE_SYSTEM_HPP