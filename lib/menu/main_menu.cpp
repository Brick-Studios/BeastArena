#include "menu/main_menu.hpp"

#include <string>
#include <functional>

#include "entities/entity_factory.hpp"
#include "scenes/scene_manager.hpp"
#include "scenes/scene.hpp"
#include "brickengine/json/json.hpp"
#include "exceptions/size_mismatch_exception.hpp"
#include "menu/button.hpp"
#include "menu/image.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "level/level.hpp"
#include "brickengine/json/json.hpp"
#include "controllers/game_controller.hpp"

MainMenu::MainMenu(int screen_width, int screen_height, GameController* game_controller) : Menu(screen_width, screen_height) {
    // General information
    this->version = 1.0;
    this->name = "Main Menu";
    this->bg_path = "colors/white.png";
    this->bg_music = "music/rainforest.mp3";

    // Buttons
    Button start_game_button = Button();
    start_game_button.texture_path = "menu/button.png";
    start_game_button.alpha = 255;
    start_game_button.x = 960;
    start_game_button.y = 340;
    start_game_button.x_scale = 400;
    start_game_button.y_scale = 100;
    start_game_button.text.text = "Play Game!";
    start_game_button.text.font_size = 72;
    start_game_button.text.color = { 255, 255, 255, 255 };
    start_game_button.text.x = 960;
    start_game_button.text.y = 335;
    start_game_button.text.x_scale = 400;
    start_game_button.text.y_scale = 100;
    const auto game_controller_pass = game_controller;
    start_game_button.on_click = [game_controller_pass]() -> void {
        game_controller_pass->startGame();
    };
    this->buttons.push_back(start_game_button);

    // Button highscore_button = Button();
    // highscore_button.texture_path = "menu/button.png";
    // highscore_button.alpha = 255;
    // highscore_button.x = 960;
    // highscore_button.y = 460;
    // highscore_button.xScale = 400;
    // highscore_button.yScale = 100;
    // highscore_button.text.text = "Highscores";
    // highscore_button.text.font_size = 72;
    // highscore_button.text.color = { 255, 255, 255, 255 };
    // highscore_button.text.x = 960;
    // highscore_button.text.y = 455;
    // highscore_button.text.x_scale = 400;
    // highscore_button.text.y_scale = 100;
    // highscore_button.on_click = []() -> void {
    //     std::cout << "Je klikt nu op de highscore knop" << std::endl;
    // };
    // this->buttons.push_back(highscore_button);

    // Button help_button = Button();
    // help_button.texture_path = "menu/button.png";
    // help_button.alpha = 255;
    // help_button.x = 960;
    // help_button.y = 580;
    // help_button.xScale = 400;
    // help_button.yScale = 100;
    // help_button.text.text = "Help";
    // help_button.text.font_size = 72;
    // help_button.text.color = { 255, 255, 255, 255 };
    // help_button.text.x = 960;
    // help_button.text.y = 575;
    // help_button.text.x_scale = 200;
    // help_button.text.y_scale = 100;
    // help_button.on_click = []() -> void {
    //     std::cout << "Je klikt nu op de help knop" << std::endl;
    // };
    // this->buttons.push_back(help_button);

    // Button credits_button = Button();
    // credits_button.texture_path = "menu/button.png";
    // credits_button.alpha = 255;
    // credits_button.x = 960;
    // credits_button.y = 700;
    // credits_button.x_scale = 400;
    // credits_button.y_scale = 100;
    // credits_button.text.text = "Credits";
    // credits_button.text.font_size = 72;
    // credits_button.text.color = { 255, 255, 255, 255 };
    // credits_button.text.x = 960;
    // credits_button.text.y = 695;
    // credits_button.text.x_scale = 300;
    // credits_button.text.y_scale = 100;
    // credits_button.on_click = []() -> void {
    //     std::cout << "Je klikt nu op de credits knop" << std::endl;
    // };
    // this->buttons.push_back(credits_button);

    // Button exit_button = Button();
    // exit_button.texture_path = "menu/button.png";
    // exit_button.alpha = 255;
    // exit_button.x = 960;
    // exit_button.y = 820;
    // exit_button.x_scale = 400;
    // exit_button.y_scale = 100;
    // exit_button.text.text = "Exit game";
    // exit_button.text.font_size = 72;
    // exit_button.text.color = { 255, 255, 255, 255 };
    // exit_button.text.x = 960;
    // exit_button.text.y = 815;
    // exit_button.text.x_scale = 400;
    // exit_button.text.y_scale = 100;
    // exit_button.on_click = []() -> void {
    //     std::cout << "Je klikt nu op de exit knop" << std::endl;
    // };
    // this->buttons.push_back(exit_button);

    // Images
    Image logo = Image();
    logo.texture_path = "menu/logo.png";
    logo.alpha = 255;
    logo.x = 960;
    logo.y = 210;
    logo.x_scale = 680;
    logo.y_scale = 106;
    this->images.push_back(logo);
}