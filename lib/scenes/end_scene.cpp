#include "scenes/end_scene.hpp"

#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"
#include "brickengine/components/player_component.hpp"

#include "components/despawn_component.hpp"
#include "components/health_component.hpp"
#include "components/stats_component.hpp"
#include "components/pickup_component.hpp"

EndScene::EndScene(EntityFactory& entity_factory, BrickEngine& engine) : 
    BeastScene<EndScene>(entity_factory, engine, WIDTH, HEIGHT) {};

void EndScene::performPrepare() {
    entity_components = std::make_unique<std::vector<EntityComponents>>();;

    // Background
    entity_components->push_back(factory.createImage("backgrounds/arena.jpg", WIDTH / 2, HEIGHT / 2, WIDTH, HEIGHT, getRelativeModifier(), Layers::Background, 255));

    // Text
    entity_components->push_back(factory.createText("All players need to pick up a skip sign to continue", { 255, 255, 255, 255 }, 72, WIDTH / 2, 100, 800, 100, getRelativeModifier(), Layers::Lowground));

    // Walls
    entity_components->push_back(factory.createPlatform(WIDTH / 2, 1077, 1920, 10, getRelativeModifier(), "colors/black.jpg", 255));
    entity_components->push_back(factory.createPlatform(-150, 540, 300, 1080, getRelativeModifier(), "colors/black.jpg", 255));
    entity_components->push_back(factory.createPlatform(2070, 540, 300, 1080, getRelativeModifier(), "colors/black.jpg", 255));

    // Letters
    entity_components->push_back(factory.createPlatform(950, 930, 200, 300, getRelativeModifier(), "levels/end_game/first_platform.png", 255));
    entity_components->push_back(factory.createPlatform(760, 990, 200, 175, getRelativeModifier(), "levels/end_game/second_platform.png", 255));
    entity_components->push_back(factory.createPlatform(1155, 1025, 220, 100, getRelativeModifier(), "levels/end_game/third_platform.png", 255));

    // Prepare playerspawns;
    player_spawns.push_back({ 950, 500});
    player_spawns.push_back({ 750, 500});
    player_spawns.push_back({ 1150, 500});
    player_spawns.push_back({ 1700, 500});

    // Spawn the trophy
    entity_components->push_back(factory.createTrophy(WIDTH / 2, 400, 50, 75, getRelativeModifier(), Layers::Gadgets, 255));


    auto& em = factory.getEntityManager();
    auto entities_with_player = em.getEntitiesByComponent<PlayerComponent>();
    // Spawn the signs (there are always 2)
    entity_components->push_back(factory.createReadySign(200, 980, 80, 40, getRelativeModifier(), Layers::Gadgets, 255));
    entity_components->push_back(factory.createReadySign(300, 980, 80, 40, getRelativeModifier(), Layers::Gadgets, 255));
    if(entities_with_player.size() > 2) {
        entity_components->push_back(factory.createReadySign(1720, 980, 80, 40, getRelativeModifier(), Layers::Foreground, 255));
        if(entities_with_player.size() > 3)
            entity_components->push_back(factory.createReadySign(1620, 980, 80, 40, getRelativeModifier(), Layers::Foreground, 255));
    }
}

void EndScene::start() {
    auto& em = factory.getEntityManager();
    auto entities_with_player = em.getEntitiesByComponent<PlayerComponent>();

    // entity_id and points
    std::vector<std::pair<int, int>> results;
  
    for (auto& [ entity_id, player ] : entities_with_player) {
        for (auto& child : em.getChildren(entity_id))
            em.moveOutOfParentsHouse(child);
        em.moveOutOfParentsHouse(entity_id);

        auto stats = em.getComponent<StatsComponent>(entity_id);
        results.push_back(std::make_pair(entity_id, stats->levels_won));

        // Revive the player
        auto health_component = em.getComponent<HealthComponent>(entity_id);
        (*health_component->revive)(entity_id);
        auto despawn_component = em.getComponent<DespawnComponent>(entity_id);
        despawn_component->despawn_on_out_of_screen = true;
    }

    std::sort(results.begin(), results.end(), [](auto lhs, auto rhs) {
        return lhs.second > rhs.second;
    });


    int spawn = 0;
    int count = 1;  
    int y = 200;
    for(auto [entity_id, result] : results) {
        // Put player on the right position
        auto player_spawn = player_spawns.at(spawn);
        auto transformComponent = em.getComponent<TransformComponent>(entity_id);
        if(transformComponent) {
            transformComponent->x_pos = player_spawn.x / getRelativeModifier();
            transformComponent->y_pos = player_spawn.y / getRelativeModifier();
        }
        ++spawn;
        // Render the leaderboard
        auto stats_component = em.getComponent<StatsComponent>(entity_id);
        auto player_component = em.getComponent<PlayerComponent>(entity_id);
        // Check how many dots we have to insert
        auto text = std::to_string(count) + ": " + player_component->name;
        auto level_digits = 1;
        if(stats_component->levels_won > 0)
            level_digits = floor(log10(stats_component->levels_won));
        auto dots_to_add = 60 - level_digits - player_component->name.size() - text.size();
        while(text.size() < dots_to_add) {
            text = text + ".";
        }
        text = text + std::to_string(result);

        auto comps = factory.createText(text, { 255, 255, 255, 255 }, 50, this->screen_width / 2, y, 750, 50, 1, Layers::Foreground);
        comps.tags.push_back(std::to_string(player_component->player_id) + "leaderboard");
        factory.addToEntityManager(std::move(comps));

        ++count;
        y += 50;
    }
    engine.getSoundManager().playMusic("music/endgame.mp3");
}

void EndScene::leave() {
    engine.getSoundManager().stopMusic();
}
