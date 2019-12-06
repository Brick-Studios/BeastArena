#include "systems/damage_system.hpp"
#include "components/health_component.hpp"
#include "components/stats_component.hpp"

DamageSystem::DamageSystem(CollisionDetector2& cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void DamageSystem::update(double) {
    auto entities_with_damage_component = entityManager->getEntitiesByComponent<DamageComponent>();

    for (auto& [entity_id, damage_comp]: entities_with_damage_component) {
        auto collisions = collision_detector.detectCollision(entity_id);
        if (!collisions.empty()) {
            bool should_despawn = false;
            for (const Collision& c : collisions) {
                if (c.is_trigger) continue;
                if (c.opposite_id == damage_comp->damage_dealer_entity_id) continue;
                this->collide(*damage_comp, c);
                should_despawn = true;
            }
            if (damage_comp->despawn && should_despawn)
                entityManager->removeEntity(entity_id);
        }
    }
}

void DamageSystem::collide(DamageComponent& damage_comp, const Collision& collision) {
    auto health_hit_entity = entityManager->getComponent<HealthComponent>(collision.opposite_id);
    if (health_hit_entity) {
        health_hit_entity->health -= damage_comp.damage;
        // u ded m8
        if (health_hit_entity->health <= 0) {
            auto stats_hit_entity = entityManager->getComponent<StatsComponent>(collision.opposite_id);
            // User's stats are updated with one dead added.
            stats_hit_entity->deaths+=1;

            health_hit_entity->on_death(collision.opposite_id);
            if(damage_comp.damage_dealer_entity_id){
                auto points_gainer = entityManager->getComponent<StatsComponent>(*damage_comp.damage_dealer_entity_id);
                if(points_gainer && health_hit_entity->points_on_kill.has_value()){
                    // User's stats are updated with kill points.
                    ++points_gainer->kills;
                }else{
                    // User killed a critter and that stat is updated.
                    points_gainer->killed_critters += 1;
                }
            }
        }
    }
}