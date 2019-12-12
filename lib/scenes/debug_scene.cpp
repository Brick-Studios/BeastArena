#include "scenes/debug_scene.hpp"

#include "brickengine/json/json.hpp"
#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "components/despawn_component.hpp"
#include "scenes/exceptions/not_enough_player_spawns_exception.hpp"
#include "brickengine/std/random.hpp"

DebugScene::DebugScene(EntityFactory& factory, BrickEngine& engine, Json json)
    : json(json), BeastScene<DebugScene>(factory, engine, json.getInt("width"), json.getInt("height")) {
        prepared = false;
    }

void DebugScene::performPrepare() {
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

    // Preparing player spawns including the players who are about to be spawned.
    int player_count = 1;
    for(Json player_spawn_json : json.getVector("player_spawns")) { 
        PlayerSpawn player_spawn = PlayerSpawn();

        player_spawn.x = player_spawn_json.getInt("x");
        player_spawn.y = player_spawn_json.getInt("y");
        
        factory.addToEntityManager(factory.createPlayer(player_count, Character(player_count - 1), -2000, -2000));
        ++player_count;

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
}
void DebugScene::start() {
    auto& em = factory.getEntityManager();
    auto& r = Random::getInstance(); 

    // Create the background
    auto comps = factory.createImage(this->bg_path, this->width / 2, this->height / 2, this->width, this->height, getRelativeModifier(), Layers::Background, 255);
    factory.addToEntityManager(std::move(comps));

    // Load the players on the spawn locations
    auto entities_with_player = em.getEntitiesByComponent<PlayerComponent>();

    for(auto& [entity_id, player]: entities_with_player) {
        for (auto& child : em.getChildren(entity_id)) {
            em.moveOutOfParentsHouse(child);
        }
    }

    int count = 0;
    for(auto& [entity_id, player]: entities_with_player) {
        player->disabled = false;
        auto transform_component = em.getComponent<TransformComponent>(entity_id);

        auto health_component = em.getComponent<HealthComponent>(entity_id);
        (*health_component->revive)(entity_id);

        transform_component->x_pos = player_spawns[count].x / getRelativeModifier();
        transform_component->y_pos = player_spawns[count].y / getRelativeModifier();

        auto despawn_component = em.getComponent<DespawnComponent>(entity_id);
        despawn_component->despawn_on_out_of_screen = true;

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

void DebugScene::leave() {
    auto& em = factory.getEntityManager();
    auto entities_with_player = em.getEntitiesByComponent<PlayerComponent>();
    for(auto& [entity_id, player]: entities_with_player) {
        auto transform_component = em.getComponent<TransformComponent>(entity_id);
        transform_component->x_pos = -2000;
        transform_component->y_pos = -2000;
        player->disabled = true;
        auto despawn_component = em.getComponent<DespawnComponent>(entity_id);
        despawn_component->despawn_on_out_of_screen = false;
    }
    
    engine.getSoundManager().stopMusic();
}