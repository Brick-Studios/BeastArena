#include "brickengine/input.hpp"

#include "systems/spawn_system.hpp"
#include "components/spawn_component.hpp"
#include "components/weapon_component.hpp"
#include "brickengine/std/random.hpp"

SpawnSystem::SpawnSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em) {}

void SpawnSystem::update(double deltatime) {
    auto& r = Random::getInstance(); 
    auto entities_with_spawn = entityManager->getEntitiesByComponent<SpawnComponent>();
    auto entities_with_weapon = entityManager->getEntitiesByComponent<WeaponComponent>();

    for (auto& [entity_id, spawn] : entities_with_spawn) {
        if (!spawn->always_respawn) {
            // If the spawned entity is still surviving
            if (spawn->spawned_entity)
                if (entityManager->entityExists(*spawn->spawned_entity)) continue;
        }
        
        // If spawner has a child (entity spawned) : continue
        if (entityManager->getChildren(entity_id).size() > 0) continue;

        // Wait for respawn time
        if (spawn->time_elapsed < spawn->respawn_timer) {
            spawn->time_elapsed += deltatime;
        }
        // Spawn a random entity
        else {
            int random_comps_index = r.getRandomInt(0, spawn->comp_fns.size() - 1);
            auto [ comps, tags ] = spawn->comp_fns[random_comps_index]();

            int spawned_entity = entityManager->createEntity(std::move(comps), std::make_pair(entity_id, false));
            for (auto& tag : tags) {
                entityManager->setTag(spawned_entity, tag);
            }
            spawn->spawned_entity = spawned_entity;

            auto transform = entityManager->getComponent<TransformComponent>(spawned_entity);
            transform->x_pos = 0;
            transform->y_pos = -25;

            spawn->time_elapsed = 0;
        } 
    }
}
