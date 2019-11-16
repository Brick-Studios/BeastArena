#ifndef FILE_MENU_HPP
#define FILE_MENU_HPP

#include <string>
#include <vector>

#include "scenes/scene.hpp"
#include "brickengine/json/json.hpp"
#include "menu/button.hpp"
#include "menu/image.hpp"

class Menu : public Scene {
public:
    Menu(int screen_width, int screen_height) : Scene(screen_width, screen_height, 1920, 1080) {};

    // Buttons
    std::vector<Button> buttons;

    // Images
    std::vector<Image> images;
};

#endif // FILE_MENU_HPP