#include "scenes/scene_manager.hpp"

#include <utility>

#include "entities/entity_factory.hpp"
#include "brickengine/entities/entity_manager.hpp"
#include "level/level.hpp"
#include "entities/layers.hpp"
#include "level/solid.hpp"
#include "level/player_spawn.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/components/transform_component.hpp"
#include "menu/menu.hpp"
#include "components/wandering_component.hpp"

SceneManager::SceneManager(std::shared_ptr<EntityFactory> entity_factory, std::shared_ptr<EntityManager> entity_manager, BrickEngine* engine) : entity_factory(entity_factory), entity_manager(entity_manager), engine(engine) {};

void SceneManager::loadLevel(Level& level) {
    // Create the players
    current_scene_entities.insert(entity_factory->createGorilla(-300, -300, 1));
    current_scene_entities.insert(entity_factory->createPanda1(-300, -300, 2));
    current_scene_entities.insert(entity_factory->createPanda2(-300, -300, 3));
    current_scene_entities.insert(entity_factory->createPanda3(-300, -300, 4));
    current_scene_entities.insert(entity_factory->createWeapon(1000, 200, true));
    current_scene_entities.insert(entity_factory->createWeapon(1100, 200, false));
    current_scene_entities.insert(entity_factory->createWeapon(600, 200, true));
    current_scene_entities.insert(entity_factory->createWeapon(500, 200, false));

    // Create the background
    loadBackground(level.bg_path);

    // Load the players on the spawn locations
    auto entities_with_player = entity_manager->getEntitiesByComponent<PlayerComponent>();

    int count = 0;
    for(auto& [entity_id, player]: entities_with_player) {
        std::ignore = player;
        auto transform_component = entity_manager->getComponent<TransformComponent>(entity_id);

        transform_component->x_pos = level.player_spawns[count].x / level.relative_modifier;
        transform_component->y_pos = level.player_spawns[count].y / level.relative_modifier;

        ++count;
    }

    //Load the critters on the spawn locations
    for(int i = 0; i < level.critter_spawns.size(); i++) {
        current_scene_entities.insert(entity_factory->createCritter(level.critter_spawns[i].x / level.relative_modifier,
        level.critter_spawns[i].y / level.relative_modifier));
    }

    // Create the platforms
    for(Solid platform : level.solids) {
        if(platform.shape == SolidShape::RECTANGLE && platform.effect == SolidEffect::NONE) {
            int x = platform.x / level.relative_modifier;
            int y = platform.y / level.relative_modifier;
            int xScale = platform.xScale / level.relative_modifier;
            int yScale = platform.yScale / level.relative_modifier;
            current_scene_entities.insert(entity_factory->createPlatform(x, y, xScale, yScale, platform.texture_path, platform.alpha));
        }
    }

    engine->toggleCursor(false);
}

void SceneManager::loadMenu(Menu& menu) {
    // Create the background
    loadBackground(menu.bg_path);

    // Load the buttons
    for(Button button : menu.buttons) {
        auto ids = entity_factory->createButton(button, menu.relative_modifier);
        current_scene_entities.insert(ids.first);
        current_scene_entities.insert(ids.second);
    }

    // Load the images
    for(Image image : menu.images) {
        current_scene_entities.insert(entity_factory->createImage(image.texture_path, image.x / menu.relative_modifier, image.y / menu.relative_modifier, image.x_scale / menu.relative_modifier, image.y_scale / menu.relative_modifier, Layers::Middleground, image.alpha));
    }
}

void SceneManager::intermission(int timer) {
    auto entities = entity_manager->getEntitiesWithTag("intermission");
    if(!entities.empty()) {
        for(auto entity : entities) {
            entity_manager->removeEntity(entity);
            current_scene_entities.erase(entity);
        }
    }
    auto entity = entity_factory->createText(std::to_string(timer), 800, 450, 300, 300);
    current_scene_entities.insert(entity);
    entity_manager->setTag(entity, "intermission");

    engine->toggleCursor(true);
}

void SceneManager::destroyCurrentScene() {
    for(int entity_id : current_scene_entities) {
        entity_manager->removeEntity(entity_id);
    }
    current_scene_entities.clear();
}

void SceneManager::loadBackground(std::string path) {
    current_scene_entities.insert(entity_factory->createImage(path, engine->getWindowWidth() / 2, engine->getWindowHeight() / 2, engine->getWindowWidth(), engine->getWindowHeight(), Layers::Background, 255));
}
