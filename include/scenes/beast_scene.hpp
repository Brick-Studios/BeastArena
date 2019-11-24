#ifndef FILE_BEAST_SCENE_HPP
#define FILE_BEAST_SCENE_HPP

#include <string>

#include "brickengine/scenes/scene_impl.hpp"
#include "enums/game_state.hpp"
#include "entities/entity_factory.hpp"
#include "controllers/game_controller.hpp"
#include "scenes/exceptions/size_mismatch_exception.hpp"

template<typename SceneType>
class BeastScene : public SceneImpl<SceneType, GameState> {
public:
    BeastScene(EntityFactory& factory, BrickEngine& engine, int width, int height) : factory(factory), engine(engine), width(width), height(height) {}
protected:
    // General information
    // Modifier needed to create entities at the right place with the right size
    double getRelativeModifier() {
        double width_ratio = (double)width / (double)screen_width;
        double height_ratio = (double)height / (double)screen_height;
    
        if(width_ratio != height_ratio) {
            throw SizeMismatchException();
        }
        return width_ratio;
    }
    std::string bg_path;
    std::string bg_music;

    int screen_width = GameController::SCREEN_WIDTH;
    int screen_height = GameController::SCREEN_HEIGHT;
    int width;
    int height;

    EntityFactory& factory;
    BrickEngine& engine;
};

#endif // FILE_BEAST_SCENE_HPP
