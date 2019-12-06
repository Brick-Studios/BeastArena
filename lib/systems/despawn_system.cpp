#include "systems/despawn_system.hpp"
#include "components/health_component.hpp"
#include <algorithm>

DespawnSystem::DespawnSystem(CollisionDetector2& cd, std::shared_ptr<EntityManager> em,
                             int screen_width, int screen_height)
    : System(em), collision_detector(cd), screen_width(screen_width), screen_height(screen_height) {}

void DespawnSystem::update(double) {
    auto entities_with_despawn = entityManager->getEntitiesByComponent<DespawnComponent>();

    for (auto& [entity_id, despawn_comp]: entities_with_despawn) {
        if (despawn_comp->despawn_on_out_of_screen) {
            auto [position, scale] = entityManager->getAbsoluteTransform(entity_id);

            double left = position.x - (scale.x / 2);
            double right = position.x + (scale.x / 2);
            double top = position.y - (scale.y / 2);
            double bottom = position.y + (scale.y / 2 );

            if (left > screen_width || top > screen_height ||
               right < 0 || bottom < 0) {
                // Kill object. :O
                auto health = entityManager->getComponent<HealthComponent>(entity_id);
                if (health) {
                    auto transform = entityManager->getComponent<TransformComponent>(entity_id);
                    auto physics = entityManager->getComponent<PhysicsComponent>(entity_id);
                    health->health = 0;
                    transform->x_pos = -2000;
                    transform->y_pos = -2000;
                    physics->vx = 0;
                    physics->vy = 0;
                    physics->kinematic = Kinematic::IS_KINEMATIC;
                } else {
                    entityManager->removeEntity(entity_id);
                }
                continue;
            }
        }
        if (despawn_comp->despawn_on_collision) {
            if (!collision_detector.detectCollision(entity_id).empty()) {
                entityManager->removeEntity(entity_id);
            }
        }
    }
}