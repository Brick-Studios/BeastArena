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

GameController::GameController(){
    this->delta_time = 1;
    this->fps_cap = 120;
    this->fps_frame_time = 1000 / this->fps_cap;
}

int GameController::calculateFps(BrickEngine* engine, int start_time){
    this->delta_time = engine->getTicks() - start_time;
    if (this->delta_time == 0)
        this->delta_time = 1;
    if (this->delta_time < this->fps_frame_time) {
        double delay = this->fps_frame_time - this->delta_time;
        engine->delay(delay);
        this->delta_time += delay;
    }
    
    // The FPS cannot be calculated correctly because getTicks returns and integer of miliseconds. 
    // 1000 / 60 = 16.666666. But we can only caluclate 1000 / 16 = 62.5 where 0.5 is lost because of this integer precision issue.
    double fps = 1000.0 / this->delta_time;
    return (int)fps;
}

void GameController::createFpsCounter(int fps) {
    auto dst = std::unique_ptr<Rect>(new Rect { 0, 0, 75, 75});
    this->fps_counter = this->renderable_factory->createText(std::to_string(fps), 200, { 255, 255, 255 }, this->top_layer, std::move(dst));
}

void GameController::gameLoop() {
    BrickEngine engine = BrickEngine("Beast Arena", 1280, 720, { 1, 2, 3, 4 });
    this->renderable_factory = engine.getRenderableFactory();
    // Always draw the FPS counter on the highest layer
    this->top_layer = 4;
    this->createFpsCounter(0);
    

    auto dst_max = std::unique_ptr<Rect>(new Rect { -1000, 0, 1000, 330});
    std::unique_ptr<Renderable> makker_max = renderable_factory->createImage("./assets/graphics/verstappentesting.bmp", 1, std::move(dst_max));

    auto dst_vet = std::unique_ptr<Rect>(new Rect { 300, 150, 400, 600});
    std::unique_ptr<Renderable> makker_vet = renderable_factory->createImage("./assets/graphics/vettel_sochi.bmp", 2, std::move(dst_vet));

    auto dst_text = std::unique_ptr<Rect>(new Rect { 100, 100, 300, 150});
    auto text = renderable_factory->createText("Luuk is een eskimo", 200, { 255, 255, 255 }, 3, std::move(dst_text));

    Renderer* renderer = engine.getRenderer();

    while(true) {
        int start_time = engine.getTicks();
        int sum = this->delta_time * 0.5;

        makker_max->getDstRect()->x += sum;
        renderer->clearScreen();
        renderer->queueRenderable(*makker_max.get());
        renderer->queueRenderable(*makker_vet.get());
        renderer->queueRenderable(*text.get());
        renderer->queueRenderable(*fps_counter.get());
        renderer->drawScreen();

        int fps = this->calculateFps(&engine, start_time);
        this->createFpsCounter(fps);
    }
}