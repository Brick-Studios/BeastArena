#include "scenes/credits_scene.hpp"

#include "entities/entity_factory.hpp"
#include "scenes/data/menu/image.hpp"

CreditsScene::CreditsScene(EntityFactory& factory, BrickEngine& engine, GameController& game_controller) : Menu(factory, engine, WIDTH, HEIGHT), game_controller(game_controller) { }

void CreditsScene::performPrepare() {
    entity_components = std::make_unique<std::vector<EntityComponents>>();

    // Images
    Image logo = Image();
    logo.texture_path = "menu/logo.png";
    logo.alpha = 255;
    logo.x = 960;
    logo.y = 140;
    logo.x_scale = 680;
    logo.y_scale = 106;
    this->images.push_back(logo);

    // Buttons
    {
        auto on_click = [gm = &game_controller]() {
            gm->loadMainMenu();
        };
        auto comps_list = factory.createButton("Back", { 255, 255, 255, 255}, 72, "menu/button.png", 100, 95, 150, 100, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }

    // Text
    entity_components->push_back(factory.createText("Mark van der Meer - SCRUM Master", { 255, 255, 255, 255 }, 120, 960, 350, 800, 120, getRelativeModifier()));
    entity_components->push_back(factory.createText("Bram-Boris Meerlo - Version control", { 255, 255, 255, 255 }, 120, 960, 500, 950, 120, getRelativeModifier()));
    entity_components->push_back(factory.createText("Peter-Jan Gootzen - Software architect", { 255, 255, 255, 255 }, 120, 960, 650, 1000, 120, getRelativeModifier()));
    entity_components->push_back(factory.createText("Jan Schollaert - Game designer", { 255, 255, 255, 255 }, 120, 960, 800, 800, 120, getRelativeModifier()));
    entity_components->push_back(factory.createText("Luuk Santegoeds - Product owner", { 255, 255, 255, 255 }, 120, 960, 950, 820, 120, getRelativeModifier()));
}

void CreditsScene::start() {
    // Create the background
    auto comps = factory.createImage("backgrounds/pixel-forest.png", this->width / 2, this->height / 2, this->width, this->height, getRelativeModifier(), Layers::Background, 255);
    factory.addToEntityManager(std::move(comps));

    // Load the images
    for(Image image : images) {
        auto comps = factory.createImage(image.texture_path, image.x, image.y, image.x_scale, image.y_scale, getRelativeModifier(), Layers::Middleground, image.alpha);
        factory.addToEntityManager(std::move(comps));
    }

    engine.toggleCursor(true);

    if(!engine.getSoundManager().isPlaying()) {
        engine.getSoundManager().playMusic("music/main.mp3");
    }
}

void CreditsScene::leave() {

}