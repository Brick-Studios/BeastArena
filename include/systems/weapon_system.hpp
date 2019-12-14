#ifndef FILE_WEAPON_SYSTEM_HPP
#define FILE_WEAPON_SYSTEM_HPP

#include <memory>
#include "brickengine/collision_detector_2.hpp"
#include "brickengine/engine.hpp"
#include "systems/beast_system.hpp"

class WeaponSystem : public BeastSystem {
public:
    WeaponSystem(CollisionDetector2& cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef, BrickEngine& engine);
    void update(double deltatime);
private:
    CollisionDetector2& collision_detector;
    BrickEngine& engine;
};

#endif // FILE_WEAPON_SYSTEM_HPP