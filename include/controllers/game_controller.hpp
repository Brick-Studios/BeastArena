#ifndef FILE_GAME_CONTROLLER_HPP
#define FILE_GAME_CONTROLLER_HPP

#include <memory>
#include "brickengine/engine.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/rendering/renderable_factory.hpp"

class GameController {
private:
    std::unique_ptr<Renderable> fps_counter;
    double delta_time;
    int fps_cap;
    double fps_frame_time;

    RenderableFactory* renderable_factory;
    int top_layer;

    int calculateFps(BrickEngine* engine, int start_time);
    void createFpsCounter(int fps);
public:
    GameController();
    void gameLoop();
};

#endif
