#include "scenes/main_menu.hpp"

#include <string>
#include <functional>

#include "entities/entity_factory.hpp"
#include "brickengine/scenes/scene_manager.hpp"
#include "brickengine/json/json.hpp"
#include "scenes/exceptions/size_mismatch_exception.hpp"
#include "scenes/data/menu/button.hpp"
#include "scenes/data/menu/image.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "brickengine/json/json.hpp"
#include "controllers/game_controller.hpp"
#include "brickengine/components/player_component.hpp"

MainMenu::MainMenu(EntityFactory& factory, BrickEngine& engine, GameController& game_controller)
    : Menu(factory, engine, WIDTH, HEIGHT), game_controller(game_controller) { }

void MainMenu::performPrepare() {
    entity_components = std::make_unique<std::vector<EntityComponents>>();

    // Buttons
    {
        auto on_click = [gm = &game_controller]() {
            gm->loadLobby();
        };
        auto comps_list = factory.createButton("Play Game!", { 255, 255, 255, 255}, 70, "menu/button.png", 960, 350, 400, 100, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }
    {
        auto on_click = [gm = &game_controller]() {
            gm->showHighscores();
        };
        auto comps_list = factory.createButton("Highscores", { 255, 255, 255, 255}, 70, "menu/button.png", 960, 470, 400, 100, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }
    {
        auto on_click = [gm = &game_controller]() {
            gm->loadDebugger();
        };
        auto comps_list = factory.createButton("Debug level", { 255, 255, 255, 255}, 70, "menu/button.png", 960, 590, 400, 100, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }
    {
        auto on_click = [gm = &game_controller]() {
            gm->loadHelp();
        };
        auto comps_list = factory.createButton("Help", { 255, 255, 255, 255}, 70, "menu/button.png", 960, 710, 400, 100, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }
    {
        auto on_click = [gm = &game_controller]() {
            gm->loadCredits();
        };
        auto comps_list = factory.createButton("Credits", { 255, 255, 255, 255}, 70, "menu/button.png", 960, 830, 400, 100, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }
    {
        auto on_click = [gm = &game_controller]() {
            gm->exitGame();
        };
        auto comps_list = factory.createButton("Exit game", { 255, 255, 255, 255}, 70, "menu/button.png", 960, 950, 400, 100, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }

    // Images
    Image logo = Image();
    logo.texture_path = "menu/logo.png";
    logo.alpha = 255;
    logo.x = 960;
    logo.y = 140;
    logo.x_scale = 680;
    logo.y_scale = 106;
    this->images.push_back(logo);
}

void MainMenu::start() {
    // Create the background
    auto comps = factory.createImage("backgrounds/pixel-forest.png", this->width / 2, this->height / 2, this->width, this->height, getRelativeModifier(), Layers::Background, 255);
    factory.addToEntityManager(std::move(comps));

    // Load the images
    for(Image image : images) {
        auto comps = factory.createImage(image.texture_path, image.x, image.y, image.x_scale, image.y_scale, getRelativeModifier(), Layers::Middleground, image.alpha);
        factory.addToEntityManager(std::move(comps));
    }

    auto player_entities = factory.getEntityManager().getEntitiesByComponent<PlayerComponent>();
    for (auto& [entity_id, player] : player_entities) {
        factory.getEntityManager().removeEntity(entity_id);
    }

    engine.toggleCursor(true);

    if(!engine.getSoundManager().isPlaying()) {
        engine.getSoundManager().playMusic("music/main.mp3");
    }
}
void MainMenu::leave() {}
