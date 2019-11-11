#include "level/level.hpp"

#include "brickengine/json/json.hpp"
#include "exceptions/size_mismatch_exception.hpp"
#include "exceptions/not_enough_player_spawns_exception.hpp"

Level::Level(Json json, int screen_width, int screen_height) {
    this->version = json.getDouble("version");
    this->name = json.getString("name");
    this->description = json.getString("description");

    double relative_modifier = json.getInt("width") / (double)screen_width;
    if(!json.getInt("height") / screen_height == relative_modifier) {
        throw SizeMismatchException();
    }
    this->relative_modifier = relative_modifier;

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

    // Create weapon and item spawns
    for(Json gadget_spawn_json : json.getVector("gadget_spawns")) {
        GadgetSpawn gadget_spawn = GadgetSpawn();

        if(gadget_spawn_json.getString("type") == "weapon") {
            gadget_spawn.gadget_spawn_type = GadgetSpawnType::WEAPON;
        } else if(gadget_spawn_json.getString("type") == "item") {
            gadget_spawn.gadget_spawn_type = GadgetSpawnType::ITEM;
        }

        gadget_spawn.available_spawns = gadget_spawn_json.getStringVector("available_spawns");
        gadget_spawn.respawn_timer = gadget_spawn_json.getInt("respawn_timer");
        gadget_spawn.x = gadget_spawn_json.getInt("x");
        gadget_spawn.y = gadget_spawn_json.getInt("y");

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
}