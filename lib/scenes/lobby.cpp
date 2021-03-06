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
#include "components/name_selection_component.hpp"
#include "components/despawn_component.hpp"
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
        auto comps_list = factory.createButton("Start Game!", { 255, 255, 255, 255 }, 72, "menu/button.png", 960, 700, 400, 150, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }

    // Logo
    entity_components->push_back(factory.createImage("menu/logo.png", 960, 86, 680, 106, getRelativeModifier(), Layers::Middleground, 255));

    // Help text
    entity_components->push_back(factory.createText("Press GRAB to start, use movement to select and press GRAB again to confirm", { 0, 0, 0, 255 }, 72, 960, 175, 1200, 50, getRelativeModifier(), Layers::UI));

    // Player 1 selector
    entity_components->push_back(factory.createImage("menu/frame.png", 270, 400, 380, 380, getRelativeModifier(), Layers::Middleground, 255));
    // Player 1 selector background
    entity_components->push_back(factory.createImage("colors/white.png", 270, 370, 380, 320, getRelativeModifier(), Layers::Lowground, 90));
    // Player 1 left arrow character selection
    int player_1_character_left_arrow = factory.addToEntityManager(factory.createImage("arrows/left-arrow.png", 125, 345, 40, 60, getRelativeModifier(), Layers::Middleground, 0));
    // Player 1 right arrow character selection
    int player_1_character_right_arrow = factory.addToEntityManager(factory.createImage("arrows/right-arrow.png", 415, 345, 40, 60, getRelativeModifier(), Layers::Middleground, 0));
    // Player 1 left arrow name selection
    int player_1_name_left_arrow = factory.addToEntityManager(factory.createImage("arrows/left-arrow.png", 125, 465, 40, 60, getRelativeModifier(), Layers::Middleground, 0));
    // Player 1 right arrow name selection
    int player_1_name_right_arrow = factory.addToEntityManager(factory.createImage("arrows/right-arrow.png", 415, 465, 40, 60, getRelativeModifier(), Layers::Middleground, 0));

    // Player 2 selector
    entity_components->push_back(factory.createImage("menu/frame.png", 730, 400, 380, 380, getRelativeModifier(), Layers::Middleground, 255));
    // Player 2 selector background
    entity_components->push_back(factory.createImage("colors/white.png", 730, 370, 380, 320, getRelativeModifier(), Layers::Lowground, 90));
    // Player 2 left arrow
    int player_2_character_left_arrow = factory.addToEntityManager(factory.createImage("arrows/left-arrow.png", 585, 345, 40, 60, getRelativeModifier(), Layers::Middleground, 0));
    // Player 2 right arrow
    int player_2_character_right_arrow = factory.addToEntityManager(factory.createImage("arrows/right-arrow.png", 875, 345, 40, 60, getRelativeModifier(), Layers::Middleground, 0));
    // Player 2 left arrow name selection
    int player_2_name_left_arrow = factory.addToEntityManager(factory.createImage("arrows/left-arrow.png", 585, 465, 40, 60, getRelativeModifier(), Layers::Middleground, 0));
    // Player 2 right arrow name selection
    int player_2_name_right_arrow = factory.addToEntityManager(factory.createImage("arrows/right-arrow.png", 875, 465, 40, 60, getRelativeModifier(), Layers::Middleground, 0));

    // Player 3 selector
    entity_components->push_back(factory.createImage("menu/frame.png", 1190, 400, 380, 380, getRelativeModifier(), Layers::Middleground, 255));
    // Player 3 selector background
    entity_components->push_back(factory.createImage("colors/white.png", 1190, 370, 380, 320, getRelativeModifier(), Layers::Lowground, 90));
    // Player 3 left arrow
    int player_3_character_left_arrow = factory.addToEntityManager(factory.createImage("arrows/left-arrow.png", 1045, 345, 40, 60, getRelativeModifier(), Layers::Middleground, 0));
    // Player 3 right arrow
    int player_3_character_right_arrow = factory.addToEntityManager(factory.createImage("arrows/right-arrow.png", 1335, 345, 40, 60, getRelativeModifier(), Layers::Middleground, 0));
    // Player 3 left arrow name selection
    int player_3_name_left_arrow = factory.addToEntityManager(factory.createImage("arrows/left-arrow.png", 1045, 465, 40, 60, getRelativeModifier(), Layers::Middleground, 0));
    // Player 3 right arrow name selection
    int player_3_name_right_arrow = factory.addToEntityManager(factory.createImage("arrows/right-arrow.png", 1335, 465, 40, 60, getRelativeModifier(), Layers::Middleground, 0));

    // Player 4 selector
    entity_components->push_back(factory.createImage("menu/frame.png", 1650, 400, 380, 380, getRelativeModifier(), Layers::Middleground, 255));
    // Player 4 selector background
    entity_components->push_back(factory.createImage("colors/white.png", 1650, 370, 380, 320, getRelativeModifier(), Layers::Lowground, 90));
    // Player 4 left arrow)
    int player_4_character_left_arrow = factory.addToEntityManager(factory.createImage("arrows/left-arrow.png", 1505, 345, 40, 60, getRelativeModifier(), Layers::Middleground, 0));
    // Player 4 right arrow
    int player_4_character_right_arrow = factory.addToEntityManager(factory.createImage("arrows/right-arrow.png", 1795, 345, 40, 60, getRelativeModifier(), Layers::Middleground, 0));
    // Player 4 left arrow name selection
    int player_4_name_left_arrow = factory.addToEntityManager(factory.createImage("arrows/left-arrow.png", 1505, 465, 40, 60, getRelativeModifier(), Layers::Middleground, 0));
    // Player 4 right arrow name selection
    int player_4_name_right_arrow = factory.addToEntityManager(factory.createImage("arrows/right-arrow.png", 1795, 465, 40, 60, getRelativeModifier(), Layers::Middleground, 0));

    // Bottom solid
    entity_components->push_back(factory.createPlatform(960, 1085, 1920, 10, getRelativeModifier(), "colors/black.jpg", 0));

    // Left solid
    entity_components->push_back(factory.createPlatform(-5, 540, 10, 1080, getRelativeModifier(), "colors/black.jpg", 0));

    // Right solid
    entity_components->push_back(factory.createPlatform(1925, 540, 10, 1080, getRelativeModifier(), "colors/black.jpg", 0));

    // Behind button solid
    entity_components->push_back(factory.createPlatform(960, 700, 400, 150, getRelativeModifier(), "colors/black.jpg", 0));

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
    int character_selector_1_id = factory.addToEntityManager(factory.createCharacterSelector(1, 270, 350, getRelativeModifier(), player_1_character_left_arrow, player_1_character_right_arrow));
    int character_selector_2_id = factory.addToEntityManager(factory.createCharacterSelector(2, 730, 350, getRelativeModifier(), player_2_character_left_arrow, player_2_character_right_arrow));
    int character_selector_3_id = factory.addToEntityManager(factory.createCharacterSelector(3, 1190, 350, getRelativeModifier(), player_3_character_left_arrow, player_3_character_right_arrow));
    int character_selector_4_id = factory.addToEntityManager(factory.createCharacterSelector(4, 1650, 350, getRelativeModifier(), player_4_character_left_arrow, player_4_character_right_arrow));

    // Load name selection components
    int name_selector_1_id = factory.addToEntityManager(factory.createNameSelector(1, 0, 115, getRelativeModifier(), player_1_name_left_arrow, player_1_name_right_arrow));
    factory.getEntityManager().setParent(name_selector_1_id, character_selector_1_id, true);

    int name_selector_2_id = factory.addToEntityManager(factory.createNameSelector(2, 0, 115, getRelativeModifier(), player_2_name_left_arrow, player_2_name_right_arrow));
    factory.getEntityManager().setParent(name_selector_2_id, character_selector_2_id, true);

    int name_selector_3_id = factory.addToEntityManager(factory.createNameSelector(3, 0, 115, getRelativeModifier(), player_3_name_left_arrow, player_3_name_right_arrow));
    factory.getEntityManager().setParent(name_selector_3_id, character_selector_3_id, true);

    int name_selector_4_id = factory.addToEntityManager(factory.createNameSelector(4, 0, 115, getRelativeModifier(), player_4_name_left_arrow, player_4_name_right_arrow));
    factory.getEntityManager().setParent(name_selector_4_id, character_selector_4_id, true);
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

        std::string name;
        auto name_selection_components = em.getEntitiesByComponent<NameSelectionComponent>();
        for(auto& [name_selection_entity_id, name_selection_selector] : name_selection_components) {
            if(character_selection_component->player_id == name_selection_selector->player_id) {
                name = name_selection_selector->selected_name;
                break;
            }
        }

        auto comps = factory.createPlayer(character_selection_component->player_id, available_characters.at(random_index), name, 500, 500);
        factory.addToEntityManager(std::move(comps));

        available_characters.erase(available_characters.begin() + random_index);

        em.removeEntity(character_selection_component->player_entity_id);
    }

    auto player_entities = em.getEntitiesByComponent<PlayerComponent>();
    auto& input = BrickInput<PlayerInput>::getInstance();
    // Reset the ttw
    for (int i = 1; i <= 4; ++i)
        input.removeTimeToWait(i, PlayerInput::X_AXIS);


    for (auto& [ entity_id, player ] : player_entities ) {
        // removes the lobby tag from the player so that it will not be in an scene
        em.removeTag(entity_id, this->getTag());

        auto transform = em.getComponent<TransformComponent>(entity_id);
        auto physics = em.getComponent<PhysicsComponent>(entity_id);
        auto despawn = em.getComponent<DespawnComponent>(entity_id);
        transform->x_pos = -2000;
        transform->y_pos = -2000;
        physics->vx = 0;
        physics->vy = 0;
        physics->kinematic = Kinematic::IS_KINEMATIC;
        despawn->despawn_on_out_of_screen = false;
    }
}

