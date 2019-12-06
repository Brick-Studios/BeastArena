#ifndef FILE_SCORE_SCENE_HPP
#define FILE_SCORE_SCENE_HPP

#include "scenes/beast_scene.hpp"
#include "brickengine/json/json.hpp"
#include "entities/entity_factory.hpp"
#include "scenes/data/level/player_spawn.hpp"
#include "scenes/data/level/gadget_spawn.hpp"
#include "scenes/data/level/solid.hpp"
#include "scenes/data/level/critter_spawn.hpp"

class EndScene : public BeastScene<EndScene> {
public:
    EndScene(EntityFactory& entity_factory, BrickEngine& engine);
    
    static std::string getTagStatic() {
        return "EndScene";
    }
    GameState getSystemState() const {
        return GameState::EndGame;
    }
    static SceneLayer getLayerStatic() {
        return SceneLayer::Primary;
    }
    void performPrepare();
    void start();
    void leave();
private:
    static const int WIDTH = 1920;
    static const int HEIGHT = 1080;
    
    std::vector<PlayerSpawn> player_spawns;
};

#endif // FILE_SCORE_SCENE_HPP