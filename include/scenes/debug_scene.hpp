#ifndef FILE_DEBUG_SCENE_HPP
#define FILE_DEBUG_SCENE_HPP

#include "scenes/beast_scene.hpp"
#include "brickengine/json/json.hpp"
#include "scenes/data/level/player_spawn.hpp"
#include "scenes/data/level/gadget_spawn.hpp"
#include "scenes/data/level/solid.hpp"
#include "scenes/data/level/critter_spawn.hpp"

class DebugScene : public BeastScene<DebugScene> {
public:
    DebugScene(EntityFactory& factory, BrickEngine& engine, Json json);

    static std::string getTagStatic() {
        return "DebugScene";
    }
    GameState getSystemState() const {
        return GameState::LevelDebugger;
    }
    static SceneLayer getLayerStatic() {
        return SceneLayer::Primary;
    }

    void start();
    void leave();
protected:
    void performPrepare();
private:
    Json json;
    
    // General information
    double version;
    std::string name;
    std::string description;

    // Player spawns
    std::vector<PlayerSpawn> player_spawns;

    // Gadget spawns
    std::vector<GadgetSpawn> gadget_spawns;

    // Critter spawns
    std::vector<CritterSpawn> critter_spawns;

    // Solids (floors and walls)
    std::vector<Solid> solids;
};

#endif // FILE_DEBUG_SCENE_HPP