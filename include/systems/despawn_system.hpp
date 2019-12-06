#ifndef FILE_DESPAWN_SYSTEM_HPP
#define FILE_DESPAWN_SYSTEM_HPP

#include "brickengine/systems/system.hpp"
#include "brickengine/collision_detector_2.hpp"
#include "components/despawn_component.hpp"

class DespawnSystem : public System {
public:
    DespawnSystem(CollisionDetector2& cd, std::shared_ptr<EntityManager> em, int screen_width, int screen_heigth);
    void update(double delta_time);
private:
    CollisionDetector2& collision_detector;
    int screen_width, screen_height;
};

#endif // FILE_DESPAWN_SYSTEM_HPP