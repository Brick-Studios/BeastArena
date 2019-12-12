#include "scenes/pause_scene.hpp"

#include "entities/layers.hpp"
#include "brickengine/components/renderables/texture_component.hpp"

PauseScene::PauseScene(EntityFactory& factory, BrickEngine& engine, std::function<void()> resume_function, std::function<void()> exit_function) 
                        : Menu<PauseScene>(factory, engine, WIDTH, HEIGHT), resume_function(resume_function), exit_function(exit_function) {};

void PauseScene::performPrepare(){
    engine.toggleCursor(true);
    entity_components = std::make_unique<std::vector<EntityComponents>>();

    // Creates the white overlay to show the game is paused.
    entity_components->push_back(factory.createImage("/colors/white.png", (WIDTH * getRelativeModifier()) / 2, (HEIGHT * getRelativeModifier()) /2,
                                                            WIDTH * getRelativeModifier(), HEIGHT * getRelativeModifier(), getRelativeModifier(), Layers::UIBackground, 100));

    // Creation of the resume and exit button.
    auto entity_components_list_resume_button = factory.createButton("Resume", { 255, 255, 255, 255 }, 72, "menu/button.png", WIDTH / 2, 340, 400, 100, 255, getRelativeModifier(), resume_function);
    for(auto& components : entity_components_list_resume_button){
        entity_components->push_back(std::move(components));
    }
    
    auto entity_components_list_exit_button = factory.createButton("Exit", { 255, 255, 255, 255 }, 72, "menu/button.png", WIDTH / 2, 490, 400, 100, 255, getRelativeModifier(), exit_function);
    for(auto& components : entity_components_list_exit_button){
        entity_components->push_back(std::move(components));
    }

}

void PauseScene::start() {
}

void PauseScene::leave() {
    engine.toggleCursor(false);
}