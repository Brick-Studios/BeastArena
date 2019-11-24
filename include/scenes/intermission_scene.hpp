#ifndef FILE_INTERMISSION_SCENE_HPP
#define FILE_INTERMISSION_SCENE_HPP

#include "brickengine/scenes/scene_impl.hpp"
#include "brickengine/scenes/enums/scene_layer.hpp"
#include "entities/entity_factory.hpp"
#include "enums/game_state.hpp"

class IntermissionScene : public SceneImpl<IntermissionScene, GameState> {
public:
    IntermissionScene(int timer, EntityFactory& entity_factory);

    static std::string getTagStatic() {
        return "IntermissionScene";
    }
    GameState getSystemState() const {
        return GameState::InGame;
    }
    static SceneLayer getLayerStatic() {
        return SceneLayer::Secondary;
    }
    void start();
    void leave();
protected:
    void performPrepare();
private:
    EntityFactory& entity_factory;
    int timer;
};

#endif // FILE_INTERMISSION_SCENE_HPP