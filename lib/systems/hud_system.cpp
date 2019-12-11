#include "systems/hud_system.hpp"

#include "brickengine/components/player_component.hpp"
#include "components/hud_component.hpp"
#include "entities/layers.hpp"
#include "components/health_component.hpp"

#include <algorithm>

HUDSystem::HUDSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em) {}

void HUDSystem::update(double) {
    // Check if player is dead
    // If dead & death unknown --> add cross
    auto player_entities = entityManager->getEntitiesByComponent<PlayerComponent>();

    for (auto& [entity_id, player] : player_entities) {
        auto health_component = entityManager->getComponent<HealthComponent>(entity_id);
        if(health_component->health <= 0) {
            if(std::find(dead_players.begin(), dead_players.end(), player->player_id) == dead_players.end()) {
                // New death
                dead_players.push_back(player->player_id);

                auto hud_component = entityManager->getComponent<HUDComponent>(entity_id);
                entity_factory->addToEntityManager(entity_factory->createImage("menu/cross.png", hud_component->x, hud_component->y, hud_component->x_scale, hud_component->y_scale, 1, Layers::UIForeground, 255));
            }
        }
    }
}

void HUDSystem::reset() {
    dead_players.clear();
}