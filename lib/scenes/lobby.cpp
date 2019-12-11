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
#include "components/stats_component.hpp"

Lobby::Lobby(EntityFactory& factory, BrickEngine& engine, GameController& game_controller)
    : BeastScene(factory, engine, WIDTH, HEIGHT), game_controller(game_controller) { }

void Lobby::performPrepare() {
    entity_components = std::make_unique<std::vector<EntityComponents>>();
    // Create the background
    entity_components->push_back(factory.createImage("backgrounds/pixel-forest.png", this->width / 2, this->height / 2, this->width, this->height, getRelativeModifier(), Layers::Background, 255));

    // Load the buttons
    {
        // Back button
        auto on_click = [gm = &game_controller]() {
            gm->loadMainMenu();
        };
        auto comps_list = factory.createButton("Back", { 255, 255, 255, 255 }, 72, "menu/button.png", 100, 100, 150, 100, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }
    {
        // Start game button
        auto on_click = [gm = &game_controller]() {
            gm->startGame();
        };
        auto comps_list = factory.createButton("Start Game!", { 255, 255, 255, 255 }, 72, "menu/button.png", 960, 700, 350, 150, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }

    // Logo
    entity_components->push_back(factory.createImage("menu/logo.png", 960, 86, 680, 106, getRelativeModifier(), Layers::Middleground, 255));

    // Help text
    entity_components->push_back(factory.createText("Press GRAB to start, use movement to select and press GRAB again to confirm", { 0, 0, 0, 255 }, 960, 175, 1200, 50, 72, getRelativeModifier(), Layers::UI));

    // Player 1 selector
    entity_components->push_back(factory.createImage("menu/frame.png", 270, 400, 380, 380, getRelativeModifier(), Layers::Middleground, 255));
    // Player 1 selector background
    entity_components->push_back(factory.createImage("colors/white.png", 270, 370, 380, 320, getRelativeModifier(), Layers::Lowground, 90));
    // Player 1 left arrow
    entity_components->push_back(factory.createImage("arrows/left-arrow.png", 125, 380, 40, 60, getRelativeModifier(), Layers::Middleground, 255));
    // Player 1 right arrow
    entity_components->push_back(factory.createImage("arrows/right-arrow.png", 415, 380, 40, 60, getRelativeModifier(), Layers::Middleground, 255));

    // Player 2 selector
    entity_components->push_back(factory.createImage("menu/frame.png", 730, 400, 380, 380, getRelativeModifier(), Layers::Middleground, 255));
    // Player 2 selector background
    entity_components->push_back(factory.createImage("colors/white.png", 730, 370, 380, 320, getRelativeModifier(), Layers::Lowground, 90));
    // Player 2 left arrow
    entity_components->push_back(factory.createImage("arrows/left-arrow.png", 585, 380, 40, 60, getRelativeModifier(), Layers::Middleground, 255));
    // Player 2 right arrow
    entity_components->push_back(factory.createImage("arrows/right-arrow.png", 875, 380, 40, 60, getRelativeModifier(), Layers::Middleground, 255));

    // Player 3 selector
    entity_components->push_back(factory.createImage("menu/frame.png", 1190, 400, 380, 380, getRelativeModifier(), Layers::Middleground, 255));
    // Player 3 selector background
    entity_components->push_back(factory.createImage("colors/white.png", 1190, 370, 380, 320, getRelativeModifier(), Layers::Lowground, 90));
    // Player 3 left arrow
    entity_components->push_back(factory.createImage("arrows/left-arrow.png", 1045, 380, 40, 60, getRelativeModifier(), Layers::Middleground, 255));
    // Player 3 right arrow
    entity_components->push_back(factory.createImage("arrows/right-arrow.png", 1335, 380, 40, 60, getRelativeModifier(), Layers::Middleground, 255));

    // Player 4 selector
    entity_components->push_back(factory.createImage("menu/frame.png", 1650, 400, 380, 380, getRelativeModifier(), Layers::Middleground, 255));
    // Player 4 selector background
    entity_components->push_back(factory.createImage("colors/white.png", 1650, 370, 380, 320, getRelativeModifier(), Layers::Lowground, 90));
    // Player 4 left arrow)
    entity_components->push_back(factory.createImage("arrows/left-arrow.png", 1505, 380, 40, 60, getRelativeModifier(), Layers::Middleground, 255));
    // Player 4 right arrow
    entity_components->push_back(factory.createImage("arrows/right-arrow.png", 1795, 380, 40, 60, getRelativeModifier(), Layers::Middleground, 255));

    // Bottom solid
    entity_components->push_back(factory.createPlatform(960, 1085, 1920, 10, getRelativeModifier(), "colors/black.jpg", 0));

    // Left solid
    entity_components->push_back(factory.createPlatform(-5, 540, 10, 1080, getRelativeModifier(), "colors/black.jpg", 0));

    // Right solid
    entity_components->push_back(factory.createPlatform(1925, 540, 10, 1080, getRelativeModifier(), "colors/black.jpg", 0));

    // Behind button solid
    entity_components->push_back(factory.createPlatform(960, 700, 350, 150, getRelativeModifier(), "colors/black.jpg", 0));

    // Left left solid
    entity_components->push_back(factory.createPlatform(260, 900, 196, 40, getRelativeModifier(), "platforms/log.png", 255));

    // Left solid
    entity_components->push_back(factory.createPlatform(560, 700, 196, 40, getRelativeModifier(), "platforms/log.png", 255));

    // Right right solid
    entity_components->push_back(factory.createPlatform(1660, 900, 196, 40, getRelativeModifier(), "platforms/log.png", 255));

    // Right solid
    entity_components->push_back(factory.createPlatform(1360, 700, 196, 40, getRelativeModifier(), "platforms/log.png", 255));
    
    // Load critters
    entity_components->push_back(factory.createCritter(600, 800));
    entity_components->push_back(factory.createCritter(700, 800));
    entity_components->push_back(factory.createCritter(800, 800));

    // Load weapon spawners
    entity_components->push_back(factory.createSpawner(300, 1000, getRelativeModifier(), std::vector<GadgetType>{ GadgetType::Pistol, GadgetType::Rifle, GadgetType::Sniper }, 5, true));
    entity_components->push_back(factory.createSpawner(1620, 1000, getRelativeModifier(), std::vector<GadgetType>{ GadgetType::Pistol, GadgetType::Rifle, GadgetType::Sniper }, 5, true));

    // Load character selection components
    entity_components->push_back(factory.createCharacterSelector(1, 270, 400, getRelativeModifier()));
    entity_components->push_back(factory.createCharacterSelector(2, 730, 400, getRelativeModifier()));
    entity_components->push_back(factory.createCharacterSelector(3, 1190, 400, getRelativeModifier()));
    entity_components->push_back(factory.createCharacterSelector(4, 1650, 400, getRelativeModifier()));
}

void Lobby::start() {
    // Remove remaining player components
    auto& em = factory.getEntityManager();
    auto player_entities = em.getEntitiesByComponent<PlayerComponent>();

    for (auto& [ entity_id, player ] : player_entities ) {
        em.removeEntity(entity_id);
    }

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
        auto comps = factory.createPlayer(character_selection_component->player_id, available_characters.at(random_index), 500, 500);
        factory.addToEntityManager(std::move(comps));

        available_characters.erase(available_characters.begin() + random_index);

        em.removeEntity(character_selection_component->player_entity_id);
    }

    auto player_entities = em.getEntitiesByComponent<PlayerComponent>();
    for (auto& [ entity_id, player ] : player_entities ) {
        em.removeTag(entity_id, this->getTag());
    }
}

