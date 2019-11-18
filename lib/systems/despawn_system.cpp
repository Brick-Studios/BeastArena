#include "systems/despawn_system.hpp"

DespawnSystem::DespawnSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em,
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
            if(left > screen_width || top > screen_height ||
               right < 0 || bottom < 0) {
                // :O
                entityManager->removeEntity(entity_id);
                continue;
            }
        }
        if (despawn_comp->despawn_on_collision) {
            {
                auto collision = collision_detector->spaceLeft(entity_id, Axis::X, Direction::NEGATIVE);
                if (collision.space_left >= 0 && !collision.is_trigger) {
                    entityManager->removeEntity(entity_id);
                    continue;
                }
            }
            {
                auto collision = collision_detector->spaceLeft(entity_id, Axis::X, Direction::POSITIVE);
                if (collision.space_left <= 0 && !collision.is_trigger) {
                    entityManager->removeEntity(entity_id);
                    continue;
                }
            }
            {
                auto collision = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::NEGATIVE);
                if (collision.space_left >= 0 && !collision.is_trigger) {
                    entityManager->removeEntity(entity_id);
                    continue;
                }
            }
            {
                auto collision = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::POSITIVE);
                if (collision.space_left <= 0 && !collision.is_trigger) {
                    entityManager->removeEntity(entity_id);
                    continue;
                }
            }
        }
    }
}