#include "controllers/game_controller.hpp"
#include "menu/main_menu.hpp"

int main() {
    GameController gc;

    gc.loadMainMenu();

    gc.gameLoop();
    return 0;
}
