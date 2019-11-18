#include "systems/damage_system.hpp"
#include "components/health_component.hpp"

DamageSystem::DamageSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void DamageSystem::update(double) {
    auto entities_with_damage_component = entityManager->getEntitiesByComponent<DamageComponent>();
    
    for (auto& [entity_id, damage_comp]: entities_with_damage_component) {
        {
            auto collision = collision_detector->spaceLeft(entity_id, Axis::X, Direction::NEGATIVE);
            if (collision.space_left >= 0 && collision.object_id && !collision.is_trigger)
                this->collide(damage_comp, collision);
        }
        {
            auto collision = collision_detector->spaceLeft(entity_id, Axis::X, Direction::POSITIVE);
            if (collision.space_left <= 0 && collision.object_id && !collision.is_trigger)
                this->collide(damage_comp, collision);
        }
        {
            auto collision = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::NEGATIVE);
            if (collision.space_left >= 0 && collision.object_id && !collision.is_trigger)
                this->collide(damage_comp, collision);
        }
        {
            auto collision = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::POSITIVE);
            if (collision.space_left <= 0 && collision.object_id && !collision.is_trigger)
                this->collide(damage_comp, collision);
        }
    }
}

void DamageSystem::collide(DamageComponent* damage_comp, CollisionReturnValues collision) {
    auto health_hit_entity = entityManager->getComponent<HealthComponent>(*collision.object_id);
    if (health_hit_entity) {
        health_hit_entity->health -= damage_comp->damage;
        if (health_hit_entity->health <= 0) {
            // u ded m8
            health_hit_entity->on_death(*collision.object_id);
        }
    }
}