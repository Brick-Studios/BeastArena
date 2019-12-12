#ifndef FILE_ERROR_SCENE_HPP
#define FILE_ERROR_SCENE_HPP

#include <functional>

#include "scenes/beast_scene.hpp"
#include "brickengine/json/json.hpp"

class ErrorScene : public BeastScene<ErrorScene> {
public:
    ErrorScene(EntityFactory& factory, BrickEngine& engine, std::string error, std::function<void()> return_function);

    static std::string getTagStatic() {
        return "ErrorScene";
    }
    GameState getSystemState() const {
        return GameState::Error;
    }
    static SceneLayer getLayerStatic() {
        return SceneLayer::Primary;
    }

    void start();
    void leave();
protected:
    void performPrepare();
private:
    std::string error_message;
    std::function<void()> return_function;
};

#endif // FILE_ERROR_SCENE_HPP