#include "brickengine/input.hpp"
#include "brickengine/components/player_component.hpp"
#include "components/ready_component.hpp"
#include "components/stats_component.hpp"

#include "systems/ready_up_system.hpp"
#include "player_input.hpp"

ReadyUpSystem::ReadyUpSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef,
                            std::function<void()> ready_function)
    : BeastSystem(ef, em), ready_function(ready_function){}

void ReadyUpSystem::update(double delta_time){
    auto entities_with_player = entityManager->getEntitiesByComponent<PlayerComponent>();

    if(player_ready.size() == entities_with_player.size()) {
        everyone_is_ready = true;
    } else {
        everyone_is_ready = false;
        timer = 0;
    }

    auto& input = BrickInput<PlayerInput>::getInstance();

    for (auto& [entity_id, player]: entities_with_player) {
        if (player->disabled) continue;

        auto ready_component = entityManager->getComponent<ReadyComponent>(entity_id);
        auto children = entityManager->getChildren(entity_id);
        for(int id : children) {
            if(entityManager->hasTag(id, "Ready")) {
                if(!ready_component->readied_up) {
                    ready_component->readied_up = true;
                    auto stats_component = entityManager->getComponent<StatsComponent>(entity_id);
                    // Adding a new texture that shows the user has readied up in the endgame screen.
                    auto leaderboard_entity_list = entityManager->getEntitiesWithTag(std::to_string(player->player_id) + "leaderboard"); 
                    int leaderboard_entity;
                    // There is only one entity with this tag.
                    for(auto entity : leaderboard_entity_list)
                       leaderboard_entity = entity;
                    auto leaderboard_transform = entityManager->getComponent<TransformComponent>(leaderboard_entity);
                    
                    // Create the ready check texture
                    auto comps = entity_factory->createImage("menu/check.png", 0, leaderboard_transform->y_pos, 56, 44, 1, Layers::UI, 255);

                    comps.tags.push_back("player" + std::to_string(player->player_id) + "ready");
                    int ready_up_sign_id = entity_factory->addToEntityManager(std::move(comps));
                    entityManager->setParent(ready_up_sign_id, leaderboard_entity, false);
                    auto ready_up_sign_transform = entityManager->getComponent<TransformComponent>(ready_up_sign_id);
                    ready_up_sign_transform->x_pos = (leaderboard_transform->x_scale / 2) + 40;
                    ready_up_sign_transform->y_pos = 0;
                    player_ready.insert(player->player_id);
                }
            }
        }
        if(!children.size()) {
            if(player_ready.count(player->player_id)) {
                player_ready.erase(player->player_id);
                ready_component->readied_up = false;
                entityManager->removeEntitiesWithTag("player" + std::to_string(player->player_id) + "ready");
            }
        }
    }
    if(everyone_is_ready) {
        timer += delta_time;
        if(timer > 1)
            this->ready_function();
    }
}

void ReadyUpSystem::reset() {
    this->timer = 0;
    this->everyone_is_ready = false;
    player_ready.clear();
}