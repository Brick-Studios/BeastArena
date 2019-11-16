#ifndef FILE_MAIN_MENU_HPP
#define FILE_MAIN_MENU_HPP

#include <string>
#include <vector>

#include "menu/menu.hpp"
#include "entities/entity_factory.hpp"
#include "scenes/scene_manager.hpp"
#include "controllers/game_controller.hpp"

class MainMenu : public Menu {
public:
    MainMenu(int screen_width, int screen_height, GameController* game_controller);
};
#endif // FILE_MAIN_MENU_HPP