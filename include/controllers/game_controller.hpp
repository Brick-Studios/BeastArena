#ifndef FILE_GAME_CONTROLLER_HPP
#define FILE_GAME_CONTROLLER_HPP

#include <memory>
#include <vector>
#include "brickengine/systems/system.hpp"
#include "brickengine/engine.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/rendering/renderable_factory.hpp"

class GameController {
public:
    GameController();
    void gameLoop();
    void createSystems();
private:
    std::unique_ptr<BrickEngine> engine;
    std::vector<std::unique_ptr<System>> systems;

    std::unique_ptr<Renderable> fps_counter;
    double delta_time;
    int fps_cap;

    std::vector<int> layers;
    int top_layer;
};

#endif
