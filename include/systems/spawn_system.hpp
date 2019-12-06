#ifndef FILE_GADGET_RESPAWN_SYSTEM_HPP
#define FILE_GADGET_RESPAWN_SYSTEM_HPP

#include "systems/beast_system.hpp"
#include "brickengine/collision_detector_2.hpp"

class SpawnSystem : public BeastSystem {
public:
    SpawnSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef);
    void update(double delta_time);
};

#endif // FILE_GADGET_RESPAWN_SYSTEM_HPP 