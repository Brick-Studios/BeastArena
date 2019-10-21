#include "controllers/game_controller.hpp"

int main() {
    auto gc = GameController();
    gc.gameLoop();
    return 0;
}
