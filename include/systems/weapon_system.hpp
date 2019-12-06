#ifndef FILE_WEAPON_SYSTEM_HPP
#define FILE_WEAPON_SYSTEM_HPP

#include <memory>
#include "brickengine/collision_detector_2.hpp"
#include "systems/beast_system.hpp"

class WeaponSystem : public BeastSystem {
public:
    WeaponSystem(CollisionDetector2& cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef);
    void update(double deltatime);
private:
    CollisionDetector2& collision_detector;
};

#endif // FILE_WEAPON_SYSTEM_HPP