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
    this->fps_cap = 144;
    this->delta_time = 0;
    this->top_layer = 2;
    this->layers = { 1, 2, 3 };

    engine = std::make_unique<BrickEngine>("Beast Arena", 1280, 720, layers, this->fps_cap);

    createSystems();
}

void GameController::createSystems() {
    this->systems = std::vector<std::unique_ptr<System>>();
}

void GameController::gameLoop() {
    while(true) {
        std::chrono::time_point start_time = std::chrono::high_resolution_clock::now();
        engine->getRenderer()->clearScreen();
        engine->drawFpsCounter();

        for (auto& system : systems) {
            system->update(delta_time);
        }

        engine->getRenderer()->drawScreen();

        std::chrono::time_point end_time = std::chrono::high_resolution_clock::now();
        engine->delay(start_time, end_time);
        delta_time = engine->getDeltatime();
    }
}