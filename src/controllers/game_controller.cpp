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

void GameController::GameLoop() {
    BrickEngine engine = BrickEngine("Beast Arena", 1280, 720, { 1, 2, 3 });
    RenderableFactory* renderableFactory = engine.getRenderableFactory();

    auto dst_max = std::unique_ptr<Rect>(new Rect { -1000, 0, 1000, 330});
    std::unique_ptr<Renderable> makker_max = renderableFactory->createImage("./assets/graphics/verstappentesting.bmp", 1, std::move(dst_max));

    auto dst_vet = std::unique_ptr<Rect>(new Rect { 300, 150, 400, 600});
    std::unique_ptr<Renderable> makker_vet = renderableFactory->createImage("./assets/graphics/vettel_sochi.bmp", 2, std::move(dst_vet));

    auto dst_text = std::unique_ptr<Rect>(new Rect { 0, 0, 300, 150});
    auto text = renderableFactory->createText("Luuk is een eskimo", 200, { 255, 255, 255 }, 3, std::move(dst_text));

    Renderer* renderer = engine.getRenderer();

    int i = 0;
    double deltatime = 1;
    int fps = 0;
    int fps_cap = 120;
    double fps_frametime = 1000 / fps_cap;
    while(true) {
        int starttime = engine.getTicks();
        int sum = deltatime * 0.5;
        makker_max->getDstRect()->x += sum;
        renderer->clearScreen();
        renderer->queueRenderable(*makker_max.get());
        renderer->queueRenderable(*makker_vet.get());
        renderer->queueRenderable(*text.get());
        renderer->drawScreen();

        i++;

        deltatime = engine.getTicks() - starttime;
        if (deltatime == 0)
            deltatime = 1;
        if (deltatime < fps_frametime) {
            double delay = fps_frametime - deltatime;
            engine.delay(delay);
            deltatime += delay;
        }
        
        //The FPS cannot be calculated correctly because getTicks returns and integer of miliseconds. 
        //1000 / 60 = 16.666666. But we can only caluclate 1000 / 16 = 62.5 where 0.5 is lost because of this integer precision issue.
        fps = 1000.0 / deltatime;
        std::cout << "fps: " << fps << std::endl;
    }
}