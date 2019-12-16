#include "scenes/highscore_scene.hpp"
#include "brickengine/input.hpp"
#include "player_input.hpp"

HighscoreScene::HighscoreScene(EntityManager& entity_manager, EntityFactory& entity_factory, BrickEngine& engine, ScoreJson& score_json, GameController& game_controller)
    : entity_manager(entity_manager), score_json(score_json), game_controller(game_controller), 
        BeastScene<HighscoreScene>(entity_factory, engine, GameController::SCREEN_WIDTH, GameController::SCREEN_HEIGHT) {}

void HighscoreScene::performPrepare() {
    scores = score_json.readScores();

    entity_components = std::make_unique<std::vector<EntityComponents>>();
    // Create the background
    entity_components->push_back(factory.createImage("backgrounds/pixel-forest.png", this->width / 2, this->height / 2, this->width, this->height, getRelativeModifier(), Layers::Background, 255));
    // Back button
    auto on_click = [gm = &game_controller]() {
        gm->loadMainMenu();
    };
    auto comps_list = factory.createButton("Back", { 255, 255, 255, 255 }, 72, "menu/button.png", 100, 100, 150, 100, 255, getRelativeModifier(), on_click);
    for(auto& comps : comps_list)
        entity_components->push_back(std::move(comps));

    // Text
    entity_components->push_back(factory.createText("Player 1, use left and right to cycle between highscores", { 255, 255, 255, 255}, 50, this->width / 2, 100, 550, 50, getRelativeModifier(), Layers::UI));

    // Arrows
    entity_components->push_back(factory.createImage("arrows/left-arrow.png", 120, 450, 40, 60, getRelativeModifier(), Layers::Middleground, 255));
    entity_components->push_back(factory.createImage("arrows/right-arrow.png", 1480, 450, 40, 60, getRelativeModifier(), Layers::Middleground, 255));
}

void HighscoreScene::start() {
    auto& input = BrickInput<PlayerInput>::getInstance();
    input.setTimeToWait(1, PlayerInput::X_AXIS, 0.1);
}

void HighscoreScene::leave() {
    auto& input = BrickInput<PlayerInput>::getInstance();
    input.removeTimeToWait(1, PlayerInput::X_AXIS);

    entity_manager.removeEntitiesWithTag("HighscoreScene_player");
}