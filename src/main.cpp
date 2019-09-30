#include "brickengine/engine.hpp"
#include "controllers/game_controller.hpp"

int main(int argc, char** argv) {
    auto gc = new GameController();
    gc->GameLoop();
    return 0;
}
