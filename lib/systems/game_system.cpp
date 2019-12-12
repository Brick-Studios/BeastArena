
#include "systems/game_system.hpp"
#include "brickengine/std/random.hpp"
#include "components/stats_component.hpp"
#include "scenes/intermission_scene.hpp"

GameSystem::GameSystem(std::shared_ptr<EntityManager> em, GameController& gc, SceneManager<GameState>& sm)
    : entity_manager(em), game_controller(gc), scene_manager(sm), System(em) {}

void GameSystem::update(double deltatime) {
    // Checking how many players are alive.
    auto players = entity_manager->getEntitiesByComponent<PlayerComponent>();
    for (auto player : players) {
        auto health = entity_manager->getComponent<HealthComponent>(player.first);
        if (health->health <= 0 && !dead_players.count(player.first)) {
            dead_players.insert(player.first);
        }
    }

    // If there are any dead players.
    if(!dead_players.empty()) {
        // If only one player is alive.
        int count = players.size() - dead_players.size();
        if(count <= 1) {
            // Count up to 1
            timer += deltatime;

            // Intermission should show all numbers. You can not call it only in the timer because then it would count down from 2.
            if(seconds == 3 || (!scene_manager.isSceneActive<IntermissionScene>() && timer <= 1)) {
                game_controller.intermission(seconds);
            }

            // If the timer reached above 1 reset it. Update the intermission screen with the new view. 
            if (timer > 1) {
                timer = 0;
                --seconds;
                game_controller.intermission(seconds);
            }

            // Load the new level and reset all timers.
            if(seconds <= 0) {
                auto alive_player = std::find_if(players.begin(), players.end(), [dp = dead_players](std::pair<int, PlayerComponent*> player) -> bool {
                    return dp.find(player.first) == dp.end();
                });

                if(alive_player != players.end()){
                    auto stats_alive_player = entityManager->getComponent<StatsComponent>(alive_player->first);
                    ++stats_alive_player->levels_won;
                }

                game_controller.loadNextLevel();
            }
        }
    }
}

void GameSystem::reset() {
    dead_players.clear();
    timer = 0;
    seconds = 3;
}
