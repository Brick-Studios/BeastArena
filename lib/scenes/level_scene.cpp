#include "scenes/level_scene.hpp"

#include <filesystem>

#include "brickengine/json/json.hpp"
#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "components/despawn_component.hpp"
#include "scenes/exceptions/not_enough_player_spawns_exception.hpp"
#include "brickengine/std/random.hpp"
#include "brickengine/components/player_component.hpp"
#include "components/hud_component.hpp"

LevelScene::LevelScene(EntityFactory& factory, BrickEngine& engine, Json json)
    : json(json), BeastScene<LevelScene>(factory, engine, json.getInt("width"), json.getInt("height")) {
        prepared = false;
    }

void LevelScene::performPrepare() {
    entity_components = std::make_unique<std::vector<EntityComponents>>();

    this->description = json.getString("description");
    this->version = json.getDouble("version");
    this->name = json.getString("name");
    this->bg_path = json.getString("bg_path");
    this->bg_music = json.getString("bg_music");

    // Create player spawns
    if(json.getVector("player_spawns").size() < 4) {
        throw NotEnoughPlayerSpawnsException();
    }

    for(Json player_spawn_json : json.getVector("player_spawns")) { 
        PlayerSpawn player_spawn = PlayerSpawn();

        player_spawn.x = player_spawn_json.getInt("x");
        player_spawn.y = player_spawn_json.getInt("y");

        this->player_spawns.push_back(player_spawn);
    }

    // Create gadget spawns
    for(Json gadget_spawn_json : json.getVector("gadget_spawns")) {
        GadgetSpawn gadget_spawn = GadgetSpawn();

        for (const std::string& s : gadget_spawn_json.getStringVector("available_spawns")) {
            gadget_spawn.available_spawns.push_back(GadgetTypeConverter::stringToGadgetType(s));
        }
        gadget_spawn.respawn_timer = gadget_spawn_json.getInt("respawn_timer");
        gadget_spawn.x = gadget_spawn_json.getInt("x");
        gadget_spawn.y = gadget_spawn_json.getInt("y");
        gadget_spawn.always_respawn = gadget_spawn_json.getBool("always_respawn");

        this->gadget_spawns.push_back(gadget_spawn);
    }

    // Create platforms
    for(Json solid_json : json.getVector("solids")) {
        Solid solid = Solid();

        if(solid_json.getString("shape") == "rectangle") {
            solid.shape = SolidShape::RECTANGLE;
        }

        if(solid_json.getString("effect") == "none") {
            solid.effect = SolidEffect::NONE;
        }

        solid.texture_path = solid_json.getString("texture_path");
        solid.alpha = solid_json.getInt("alpha");

        solid.x = solid_json.getInt("x");
        solid.y = solid_json.getInt("y");
        solid.xScale = solid_json.getInt("xScale");
        solid.yScale = solid_json.getInt("yScale");

        this->solids.push_back(solid);
    }

    // Create critter spawns
    for(Json critter_spawn_json : json.getVector("critter_spawns")) {
        CritterSpawn critter_spawn = CritterSpawn();

        critter_spawn.x = critter_spawn_json.getInt("x");
        critter_spawn.y = critter_spawn_json.getInt("y");

        this->critter_spawns.push_back(critter_spawn);
    }

    // Load the spawners and spawn weapons
    for(int i = 0; i < gadget_spawns.size(); i++) {
        auto comps = factory.createSpawner(gadget_spawns[i].x,
            gadget_spawns[i].y,
            getRelativeModifier(),
            gadget_spawns[i].available_spawns, 
            gadget_spawns[i].respawn_timer,
            gadget_spawns[i].always_respawn);
        entity_components->push_back(std::move(comps));
    }

    // Create billboards
    for(Json billboard : json.getVector("billboards")) {
        std::string content_path;
        // The advertisement image does not exist
        if (std::filesystem::exists(billboard.getString("content_path")))
            content_path = json.getString("content_path");
        else
            content_path = "advertisement/pisswasser.png";

        // This is billboard frame specific
        int x = billboard.getInt("x");
        int y = billboard.getInt("y");
        int billboard_x_scale = billboard.getInt("x_scale");
        int billboard_y_scale = billboard.getInt("y_scale");
        int content_x_scale = billboard_x_scale * 0.965;
        int content_y_scale = billboard_y_scale * 0.658536585366;
        int alpha = billboard.getInt("alpha");

        entity_components->push_back(factory.createImage("advertisement/billboard.png", x, y, billboard_x_scale, billboard_y_scale, getRelativeModifier(), Layers::Lowground, alpha));
        entity_components->push_back(factory.createImage(content_path, x, y, content_x_scale, content_y_scale, getRelativeModifier(), Layers::Lowground, alpha));
    }

    // Create animations
    for(Json animations : json.getVector("animations")) {
        std::string texture = animations.getString("texture");
        int sprite_height = animations.getInt("sprite_height");
        int sprite_width = animations.getInt("sprite_width");
        double update_time = animations.getDouble("update_time");
        int sprite_size = animations.getInt("sprite_size");
        int x = animations.getInt("x");
        int y = animations.getInt("y");
        int x_scale = animations.getInt("x_scale");
        int y_scale = animations.getInt("y_scale");

        entity_components->push_back(factory.createImage(texture, x, y, x_scale, y_scale, getRelativeModifier(), Layers::Middleground, 255,
                                                        sprite_width, sprite_height, update_time, sprite_size));
    }

    // Create images
    for(Json image : json.getVector("images")) {
        auto texture_path = image.getString("texture_path");
        int x_pos = image.getInt("x");
        int y_pos = image.getInt("y");
        int x_scale = image.getInt("xScale");
        int y_scale = image.getInt("yScale");
        int alpha = image.getInt("alpha");
        Layers layer = static_cast<Layers>(image.getInt("layer"));

        entity_components->push_back(factory.createImage(texture_path, x_pos, y_pos, x_scale, y_scale, getRelativeModifier(), layer, alpha));
    }


    // Create HUD Components
    auto player_entities = factory.getEntityManager().getEntitiesByComponent<PlayerComponent>();

    int spacing = screen_width / (player_entities.size() + 1);
    
    std::vector<int> player_ids;
    for (auto& [entity_id, player] : player_entities) {
        player_ids.push_back(player->player_id);
    }
    sort(player_ids.begin(), player_ids.end());

    for (auto& [entity_id, player] : player_entities) {
        auto hud_component = factory.getEntityManager().getComponent<HUDComponent>(entity_id);

        std::vector<int>::iterator it = std::find(player_ids.begin(), player_ids.end(), player->player_id);
        int index = std::distance(player_ids.begin(), it);

        int x_pos = spacing + (index * spacing);
        int y_pos = screen_height * 0.07;

        hud_component->x = x_pos;
        hud_component->y = y_pos;

        double frame_modifier = 1.3;
        
        entity_components->push_back(factory.createImage("colors/white.png", hud_component->x, hud_component->y, hud_component->x_scale * frame_modifier, hud_component->y_scale * frame_modifier, 1, Layers::UIBackground, 100));
        entity_components->push_back(factory.createImage(hud_component->texture, hud_component->x, hud_component->y, hud_component->x_scale, hud_component->y_scale, 1, Layers::UI, 255));
        entity_components->push_back(factory.createImage("menu/frame2.png", hud_component->x, hud_component->y, hud_component->x_scale * frame_modifier, hud_component->y_scale * frame_modifier, 1, Layers::UI, 255));
    }
}
void LevelScene::start() {
    auto& em = factory.getEntityManager();
    auto& r = Random::getInstance();

    // Create the background
    auto comps = factory.createImage(this->bg_path, this->width / 2, this->height / 2, this->width, this->height, getRelativeModifier(), Layers::Background, 255);
    factory.addToEntityManager(std::move(comps));

    // Load the players on the spawn locations
    auto entities_with_player = em.getEntitiesByComponent<PlayerComponent>();

    for(auto& [entity_id, player]: entities_with_player) {
        for (auto& child : em.getChildren(entity_id))
            em.moveOutOfParentsHouse(child);
        em.moveOutOfParentsHouse(entity_id);

        auto despawn_component = em.getComponent<DespawnComponent>(entity_id);
        auto health_component = em.getComponent<HealthComponent>(entity_id);

        (*health_component->revive)(entity_id);
        despawn_component->despawn_on_out_of_screen = true;
    }
    int count = 0;
    for(auto& [entity_id, player]: entities_with_player) {
        auto transform_component = em.getComponent<TransformComponent>(entity_id);

        transform_component->x_pos = player_spawns[count].x / getRelativeModifier();
        transform_component->y_pos = player_spawns[count].y / getRelativeModifier();
        ++count;
    }
    
    // Load the platforms
    for(Solid platform : solids) {
        if(platform.shape == SolidShape::RECTANGLE && platform.effect == SolidEffect::NONE) {
            int x = platform.x;
            int y = platform.y;
            int xScale = platform.xScale;
            int yScale = platform.yScale;
            auto comps = factory.createPlatform(x, y, xScale, yScale, getRelativeModifier(), platform.texture_path, platform.alpha);
            factory.addToEntityManager(std::move(comps));
        }
    }

    // Load the critters on the spawn locations
    for(int i = 0; i < critter_spawns.size(); i++) {
        auto comps = factory.createCritter(critter_spawns[i].x / getRelativeModifier(),
            critter_spawns[i].y / getRelativeModifier());
        factory.addToEntityManager(std::move(comps));
    }

    engine.toggleCursor(false);
    engine.getSoundManager().playMusic(this->bg_music);
}

void LevelScene::leave() {
    auto& em = factory.getEntityManager();
    auto entities_with_player = em.getEntitiesByComponent<PlayerComponent>();
    for(auto& [entity_id, player]: entities_with_player) {
        auto transform_component = em.getComponent<TransformComponent>(entity_id);
        auto despawn_component = em.getComponent<DespawnComponent>(entity_id);
        auto physics_component = em.getComponent<PhysicsComponent>(entity_id);

        transform_component->x_pos = -2000;
        transform_component->y_pos = -2000;
        player->disabled = true;
        despawn_component->despawn_on_out_of_screen = false;
        physics_component->vx = 0;
        physics_component->vy = 0;
        physics_component->kinematic = Kinematic::IS_KINEMATIC;
    }
    
    engine.getSoundManager().stopMusic();
}
