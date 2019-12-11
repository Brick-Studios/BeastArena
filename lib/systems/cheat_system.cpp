#include "systems/cheat_system.hpp"

#include "brickengine/input.hpp"
#include "player_input.hpp"

#include "brickengine/components/player_component.hpp"
#include "brickengine/components/transform_component.hpp"
#include "components/health_component.hpp"

CheatSystem::CheatSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef, GameController& gc) : game_controller(gc), BeastSystem(ef, em) {}

void CheatSystem::update(double deltatime) {
    auto& input = BrickInput<PlayerInput>::getInstance();

    int player_id = 0;
    int entity_id = 0;
    bool init = false;
    // Only the first player currently in-game can cheat. 
    for (auto& player : entityManager->getEntitiesByComponent<PlayerComponent>()) {
        if(player.second->player_id < player_id && init) {
            player_id = player.second->player_id;
            entity_id = player.first;
        } else {
            player_id = player.second->player_id;
            entity_id = player.first;
            init = true;
        }
    }

    // Next scene
    if(input.checkInput(player_id, PlayerInput::SKIP_LEVEL)) {
        game_controller.loadNextLevel();
    }

    // Infinite Health
    if(input.checkInput(player_id, PlayerInput::INFINITE_HEALTH)) {
        auto health_component = entityManager->getComponent<HealthComponent>(entity_id);
        health_component->health = std::numeric_limits<double>::infinity();
    }

    // Weapon drop
    if(input.checkInput(player_id, PlayerInput::RANDOM_WEAPON)) {
        auto transform_component = entityManager->getComponent<TransformComponent>(entity_id);
        auto comps = entity_factory->createWeaponDrop();
        int entity_id = entity_factory->addToEntityManager(std::move(comps));
        auto weapon_transform = entityManager->getComponent<TransformComponent>(entity_id);
        weapon_transform->x_pos = transform_component->x_pos;
        weapon_transform->y_pos = 50;
    }

    // Godmode laser
    if(input.checkInput(player_id, PlayerInput::LASER_WEAPON)) {
        auto transform_component = entityManager->getComponent<TransformComponent>(entity_id);
        auto comps = entity_factory->createLaser();
        int entity_id = entity_factory->addToEntityManager(std::move(comps));
        auto weapon_transform = entityManager->getComponent<TransformComponent>(entity_id);
        weapon_transform->x_pos = transform_component->x_pos;
        weapon_transform->y_pos = 50;
    }

    // Kill everyone except the first player
    if(input.checkInput(player_id, PlayerInput::KILL_EVERYONE_EXCEPT_YOURSELF)) {
        for(auto& player : entityManager->getEntitiesByComponent<PlayerComponent>()) {
            if(player_id != player.second->player_id) {
                // Die Die Die....
                auto health_component = entityManager->getComponent<HealthComponent>(player.first);
                health_component->health = 0;
                health_component->on_death(player.first);
            }
        }
    }
}