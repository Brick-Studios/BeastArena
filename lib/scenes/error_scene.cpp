#include "scenes/error_scene.hpp"

ErrorScene::ErrorScene(EntityFactory& factory, BrickEngine& engine, std::string error, std::function<void()> return_function) 
                        : BeastScene<ErrorScene>(factory, engine, GameController::SCREEN_WIDTH, GameController::SCREEN_HEIGHT), error_message(error), return_function(return_function) {}

void ErrorScene::performPrepare(){
    engine.toggleCursor(true);
    entity_components = std::make_unique<std::vector<EntityComponents>>();

    // Create the background.
    entity_components->push_back(factory.createImage("colors/black.jpg", width / 2, height / 2, width, height, getRelativeModifier(), Layers::Background, 255));

    // Add the back button
    auto comps_list = factory.createButton("Back", { 255, 255, 255, 255 }, 72, "menu/button.png", 100, 100, 150, 100, 255, getRelativeModifier(), return_function);
    for(auto& comps : comps_list) {
        entity_components->push_back(std::move(comps));
    }

    // Add the error message to the scene.
    entity_components->push_back(factory.createText("Error:", {255,179,179,255}, 100, width / 2, (height / 2) - 100, (width / 80) * std::string("Error: ").size(), 100, getRelativeModifier(), Layers::UI));
    entity_components->push_back(factory.createText(this->error_message, {255,179,179,0}, 100, width / 2, height / 2, (width / 100) * error_message.size(), 100, getRelativeModifier(), Layers::UI));

    // Add the error message to the scene.
    std::string return_message = "Press F6 to reload or press the back button.";
    entity_components->push_back(factory.createText(return_message, {255,255, 255,255}, 100, width / 2, height - 100, (width / 100) * return_message.size(), 70, getRelativeModifier(), Layers::UI));
}

void ErrorScene::start() {

}

void ErrorScene::leave() {
    engine.toggleCursor(false);
}