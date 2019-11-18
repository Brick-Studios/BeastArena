#include "controllers/game_controller.hpp"
#include "menu/main_menu.hpp"

int main() {
    GameController gc;

    MainMenu main_menu { gc.getScreenWidth(), gc.getScreenHeight(), &gc };
    gc.getSceneManager().loadMenu(main_menu);

    gc.gameLoop();
    return 0;
}
