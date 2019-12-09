#ifndef FILE_HELP_SCENE_HPP
#define FILE_HELP_SCENE_HPP

#include "scenes/menu.hpp"

class HelpScene : public Menu<HelpScene> {
public:
    HelpScene(EntityFactory& factory, BrickEngine& engine, GameController& game_controller);

    static std::string getTagStatic() {
        return "HelpScene";
    }
    GameState getSystemState() const {
        return GameState::Menu;
    }
    static SceneLayer getLayerStatic() {
        return SceneLayer::Primary;
    }

    void start();
    void leave();
protected:
    void performPrepare();
private:
    static const int WIDTH = 1920;
    static const int HEIGHT = 1080;
    GameController& game_controller;
};

#endif // FILE_HELP_SCENE_HPP