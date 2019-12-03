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
        
        // If spawner has a child (gadget spawned) : continue
        if (entityManager->getChildren(entity_id).size() > 0) continue;

        // Wait for respawn time
        if (spawn->time_elapsed < spawn->respawn_timer) {
            spawn->time_elapsed += deltatime;
        }
        // Spawn a random gadget (weapon for now)
        else {
            int random_gadget = r.getRandomInt(0, spawn->comp_fns.size() - 1);
            auto gadget_comps = spawn->comp_fns[random_gadget]();

            int spawned_gadget_id = entityManager->createEntity(std::move(gadget_comps), std::make_pair(entity_id, false));
            spawn->spawned_entity = spawned_gadget_id;

            auto transform = entityManager->getComponent<TransformComponent>(spawned_gadget_id);
            transform->x_pos = 0;
            transform->y_pos = -25;

            spawn->time_elapsed = 0;
        } 
    }
}
