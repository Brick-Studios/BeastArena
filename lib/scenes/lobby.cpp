#include "scenes/lobby.hpp"

#include <string>
#include <functional>
#include <vector>

#include "entities/entity_factory.hpp"
#include "brickengine/scenes/scene_manager.hpp"
#include "brickengine/json/json.hpp"
#include "scenes/exceptions/size_mismatch_exception.hpp"
#include "scenes/data/menu/button.hpp"
#include "scenes/data/menu/image.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "brickengine/json/json.hpp"
#include "brickengine/components/player_component.hpp"
#include "controllers/game_controller.hpp"
#include "brickengine/input.hpp"
#include "player_input.hpp"
#include "components/character_selection_component.hpp"
#include "brickengine/std/random.hpp"
#include "enums/gadget_type.hpp"

Lobby::Lobby(EntityFactory& factory, BrickEngine& engine, GameController& game_controller)
    : BeastScene(factory, engine, WIDTH, HEIGHT), game_controller(game_controller) { }

void Lobby::start() {
    // Create the background
    factory.createImage("backgrounds/pixel-forest.png", this->screen_width / 2, this->screen_height / 2, this->screen_width, this->screen_height, Layers::Background, 255);

    // Load the buttons
    Button back_button = Button();
    back_button.texture_path = "menu/button.png";
    back_button.alpha = 255;
    back_button.x = 100;
    back_button.y = 100;
    back_button.x_scale = 150;
    back_button.y_scale = 100;
    back_button.text.text = "Back";
    back_button.text.font_size = 72;
    back_button.text.color = { 255, 255, 255, 255 };
    back_button.text.x = 100;
    back_button.text.y = 95;
    back_button.text.x_scale = 150;
    back_button.text.y_scale = 100;
    back_button.on_click = [gm = &game_controller]() {
        gm->loadMainMenu();
    };
    factory.createButton(back_button, getRelativeModifier());

    Button start_game_button = Button();
    start_game_button.texture_path = "menu/button.png";
    start_game_button.alpha = 255;
    start_game_button.x = 960;
    start_game_button.y = 700;
    start_game_button.x_scale = 350;
    start_game_button.y_scale = 150;
    start_game_button.text.text = "Start Game!";
    start_game_button.text.font_size = 72;
    start_game_button.text.color = { 255, 255, 255, 255 };
    start_game_button.text.x = 960;
    start_game_button.text.y = 695;
    start_game_button.text.x_scale = 450;
    start_game_button.text.y_scale = 150;
    start_game_button.on_click = [gm = &game_controller]() {
        gm->startGame();
    };
    factory.createButton(start_game_button, getRelativeModifier());

    // Logo
    factory.createImage("menu/logo.png", 960 / getRelativeModifier(), 86 / getRelativeModifier(), 680 / getRelativeModifier(), 106 / getRelativeModifier(), Layers::Middleground, 255);

    // Help text
    factory.createText("Press GRAB to start, use movement to select and press GRAB again to confirm", 960 / getRelativeModifier(), 175 / getRelativeModifier(), 1200 / getRelativeModifier(), 50 / getRelativeModifier(), 72, { 0, 0, 0, 255});

    // Player 1 selector
    factory.createImage("menu/frame.png", 270 / getRelativeModifier(), 400 / getRelativeModifier(), 380 / getRelativeModifier(), 380 / getRelativeModifier(), Layers::Middleground, 255);
    // Player 1 selector background
    factory.createImage("colors/white.png", 270 / getRelativeModifier(), 370 / getRelativeModifier(), 380 / getRelativeModifier(), 320 / getRelativeModifier(), Layers::Lowground, 90);
    // Player 1 left arrow
    factory.createImage("arrows/left-arrow.png", 125 / getRelativeModifier(), 380 / getRelativeModifier(), 40 / getRelativeModifier(), 60 / getRelativeModifier(), Layers::Middleground, 255);
    // Player 1 right arrow
    factory.createImage("arrows/right-arrow.png", 415 / getRelativeModifier(), 380 / getRelativeModifier(), 40 / getRelativeModifier(), 60 / getRelativeModifier(), Layers::Middleground, 255);

    // Player 2 selector
    factory.createImage("menu/frame.png", 730 / getRelativeModifier(), 400 / getRelativeModifier(), 380 / getRelativeModifier(), 380 / getRelativeModifier(), Layers::Middleground, 255);
    // Player 2 selector background
    factory.createImage("colors/white.png", 730 / getRelativeModifier(), 370 / getRelativeModifier(), 380 / getRelativeModifier(), 320 / getRelativeModifier(), Layers::Lowground, 90);
    // Player 2 left arrow
    factory.createImage("arrows/left-arrow.png", 585 / getRelativeModifier(), 380 / getRelativeModifier(), 40 / getRelativeModifier(), 60 / getRelativeModifier(), Layers::Middleground, 255);
    // Player 2 right arrow
    factory.createImage("arrows/right-arrow.png", 875 / getRelativeModifier(), 380 / getRelativeModifier(), 40 / getRelativeModifier(), 60 / getRelativeModifier(), Layers::Middleground, 255);

    // Player 3 selector
    factory.createImage("menu/frame.png", 1190 / getRelativeModifier(), 400 / getRelativeModifier(), 380 / getRelativeModifier(), 380 / getRelativeModifier(), Layers::Middleground, 255);
    // Player 3 selector background
    factory.createImage("colors/white.png", 1190 / getRelativeModifier(), 370 / getRelativeModifier(), 380 / getRelativeModifier(), 320 / getRelativeModifier(), Layers::Lowground, 90);
    // Player 3 left arrow
    factory.createImage("arrows/left-arrow.png", 1045 / getRelativeModifier(), 380 / getRelativeModifier(), 40 / getRelativeModifier(), 60 / getRelativeModifier(), Layers::Middleground, 255);
    // Player 3 right arrow
    factory.createImage("arrows/right-arrow.png", 1335 / getRelativeModifier(), 380 / getRelativeModifier(), 40 / getRelativeModifier(), 60 / getRelativeModifier(), Layers::Middleground, 255);

    // Player 4 selector
    factory.createImage("menu/frame.png", 1650 / getRelativeModifier(), 400 / getRelativeModifier(), 380 / getRelativeModifier(), 380 / getRelativeModifier(), Layers::Middleground, 255);
    // Player 4 selector background
    factory.createImage("colors/white.png", 1650 / getRelativeModifier(), 370 / getRelativeModifier(), 380 / getRelativeModifier(), 320 / getRelativeModifier(), Layers::Lowground, 90);
    // Player 4 left arrow
    factory.createImage("arrows/left-arrow.png", 1505 / getRelativeModifier(), 380 / getRelativeModifier(), 40 / getRelativeModifier(), 60 / getRelativeModifier(), Layers::Middleground, 255);
    // Player 4 right arrow
    factory.createImage("arrows/right-arrow.png", 1795 / getRelativeModifier(), 380 / getRelativeModifier(), 40 / getRelativeModifier(), 60 / getRelativeModifier(), Layers::Middleground, 255);

    // Bottom solid
    factory.createPlatform(960 / getRelativeModifier(), 1085 / getRelativeModifier(), 1920 / getRelativeModifier(), 10 / getRelativeModifier(), "colors/black.jpg", 0);

    // Left solid
    factory.createPlatform(-5 / getRelativeModifier(), 540 / getRelativeModifier(), 10 / getRelativeModifier(), 1080 / getRelativeModifier(), "colors/black.jpg", 0);

    // Right solid
    factory.createPlatform(1925 / getRelativeModifier(), 540 / getRelativeModifier(), 10 / getRelativeModifier(), 1080 / getRelativeModifier(), "colors/black.jpg", 0);

    // Behind button solid
    factory.createPlatform(960 / getRelativeModifier(), 700 / getRelativeModifier(), 350 / getRelativeModifier(), 150 / getRelativeModifier(), "colors/black.jpg", 0);

    // Left left solid
    factory.createPlatform(260 / getRelativeModifier(), 900 / getRelativeModifier(), 196 / getRelativeModifier(), 40 / getRelativeModifier(), "platforms/log.png", 255);

    // Left solid
    factory.createPlatform(560 / getRelativeModifier(), 700 / getRelativeModifier(), 196 / getRelativeModifier(), 40 / getRelativeModifier(), "platforms/log.png", 255);

    // Right right solid
    factory.createPlatform(1660 / getRelativeModifier(), 900 / getRelativeModifier(), 196 / getRelativeModifier(), 40 / getRelativeModifier(), "platforms/log.png", 255);

    // Right solid
    factory.createPlatform(1360 / getRelativeModifier(), 700 / getRelativeModifier(), 196 / getRelativeModifier(), 40 / getRelativeModifier(), "platforms/log.png", 255);
    
    // Load critters
    factory.createCritter(600 / getRelativeModifier(), 800 / getRelativeModifier());
    factory.createCritter(700 / getRelativeModifier(), 800 / getRelativeModifier());
    factory.createCritter(800 / getRelativeModifier(), 800 / getRelativeModifier());

    // Load weapon spawners
    factory.createSpawner(300 / getRelativeModifier(), 1080 / getRelativeModifier(), std::vector<GadgetType>{ GadgetType::Pistol, GadgetType::Rifle, GadgetType::Sniper }, 5, true);
    factory.createSpawner(1620 / getRelativeModifier(), 1080 / getRelativeModifier(), std::vector<GadgetType>{ GadgetType::Pistol, GadgetType::Rifle, GadgetType::Sniper }, 5, true);

    // Load character selection components
    factory.createCharacterSelector(1, 270 / getRelativeModifier(), 400 / getRelativeModifier());
    factory.createCharacterSelector(2, 730 / getRelativeModifier(), 400 / getRelativeModifier());
    factory.createCharacterSelector(3, 1190 / getRelativeModifier(), 400 / getRelativeModifier());
    factory.createCharacterSelector(4, 1650 / getRelativeModifier(), 400 / getRelativeModifier());

    engine.toggleCursor(true);

    if(!engine.getSoundManager().isPlaying()) {
        engine.getSoundManager().playMusic("music/main.mp3");
    }

    auto& input = BrickInput<PlayerInput>::getInstance();
    input.setTimeToWait(1, PlayerInput::X_AXIS, 0.1);
    input.setTimeToWait(2, PlayerInput::X_AXIS, 0.1);
    input.setTimeToWait(3, PlayerInput::X_AXIS, 0.1);
    input.setTimeToWait(4, PlayerInput::X_AXIS, 0.1);
}

void Lobby::leave() {
    auto& em = factory.getEntityManager();
    auto character_selection_entities = em.getEntitiesByComponent<CharacterSelectionComponent>();

    auto available_characters = factory.getAvailableCharacters();
    std::vector<int> need_character_components;
    for (auto& [ entity_id, character_selection ] : character_selection_entities ) {
        if(character_selection->joined && character_selection->picked) {
            if(character_selection->selected_character != Character::RANDOM) {
                available_characters.erase(std::remove(available_characters.begin(), available_characters.end(), character_selection->selected_character), available_characters.end());
            } else {
                need_character_components.push_back(entity_id);
            }
        }
    }

    for(int entity_id : need_character_components) {
        auto& random = Random::getInstance();
        auto random_index = random.getRandomInt(0, available_characters.size() - 1);

        auto character_selection_component = em.getComponent<CharacterSelectionComponent>(entity_id);
        factory.createPlayer(character_selection_component->player_id, available_characters.at(random_index), 500, 500);

        available_characters.erase(available_characters.begin() + random_index);

        em.removeEntity(character_selection_component->player_entity_id);
    }

    auto player_entities = em.getEntitiesByComponent<PlayerComponent>();

    for (auto& [ entity_id, player ] : player_entities ) {
        em.removeTag(entity_id, this->getTag());
    }
}

void Lobby::performPrepare() { }