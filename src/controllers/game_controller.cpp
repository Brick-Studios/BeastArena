#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
using namespace std::chrono_literals;

#include "controllers/game_controller.hpp"
#include "brickengine/engine.hpp"
#include "brickengine/rendering/renderer.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/rendering/renderable_factory.hpp"

GameController::GameController() {
    this->delta_time = 1;
    this->fps_cap = 60;
    this->fps_frame_time = 1000 / this->fps_cap;
    this->top_layer = 2;
    this->layers = { 1, 2, 3 };

    engine = std::make_unique<BrickEngine>("Beast Arena", 1280, 720, layers);

    createSystems();
    createFpsCounter(0);
}

int GameController::calculateFps(int start_time){
    int ticks = engine->getTicks();
    double delta_time_millisecs = ticks - start_time;
    if (delta_time_millisecs == 0)
        delta_time_millisecs = 1;
    if (delta_time_millisecs < fps_frame_time) {
        double delay = fps_frame_time - delta_time_millisecs;
        engine->delay(delay);
        delta_time_millisecs = delta_time_millisecs + delay;
    }

    delta_time = delta_time_millisecs / 1000;

    // The FPS cannot be calculated correctly because getTicks returns and integer of miliseconds. 
    // 1000 / 60 = 16.666666, here we lose 0.666 per. But we can only caluclate 1000 / 16 = 62.5. Where 0.5 nd  is lost because of this integer precision issue.
    double fps = 1000.0 / delta_time_millisecs;
    return (int)fps;
}

void GameController::createFpsCounter(int fps) {
    auto dst = std::unique_ptr<Rect>(new Rect { 0, 0, 75, 75});
    this->fps_counter.~unique_ptr();
    this->fps_counter = engine->getRenderableFactory()->createText(std::to_string(fps), 24, { 255, 255, 255, 0}, this->top_layer, std::move(dst));
}

void GameController::createSystems() {
    this->systems = std::vector<std::unique_ptr<System>>();
}

void GameController::gameLoop() {
    while(true) {
        int start_time = engine->getTicks();

        engine->getRenderer()->clearScreen();

        for (auto& system : systems) {
            system->update(delta_time);
        }

        engine->getRenderer()->queueRenderable(fps_counter.get());
        engine->getRenderer()->drawScreen();

        int fps = this->calculateFps(start_time);
        this->createFpsCounter(fps);
    }
}