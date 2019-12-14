#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <utility>
#include <vector>
#include <deque>
#include <filesystem>
#include <map>
#include <algorithm>
#include <unordered_map>
using namespace std::chrono_literals;

#include "controllers/game_controller.hpp"
#include "brickengine/engine.hpp"
#include "brickengine/rendering/renderer.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/rendering/renderable_factory.hpp"
#include "brickengine/input.hpp"
#include "brickengine/std/random.hpp"
#include "brickengine/input_keycode.hpp"
#include "brickengine/json/json.hpp"
#include "brickengine/scenes/enums/scene_layer.hpp"
#include "brickengine/json/exceptions/no_valid_json_or_path_exception.hpp"
#include "brickengine/json/exceptions/object_or_type_exception.hpp"

// Systems
#include "brickengine/systems/rendering_system.hpp"
#include "brickengine/systems/physics_system.hpp"
#include "brickengine/systems/displacement_system.hpp"
#include "brickengine/systems/animation_system.hpp"

#include "systems/pickup_system.hpp"
#include "systems/click_system.hpp"
#include "systems/weapon_system.hpp"
#include "systems/damage_system.hpp"
#include "systems/despawn_system.hpp"
#include "systems/movement_system.hpp"
#include "systems/critter_system.hpp"
#include "systems/game_system.hpp"
#include "systems/ready_up_system.hpp"
#include "systems/highscore_system.hpp"
#include "systems/lobby_system.hpp"
#include "systems/spawn_system.hpp"
#include "systems/debug_system.hpp"
#include "systems/cheat_system.hpp"
#include "systems/hud_system.hpp"
#include "systems/pause_system.hpp"
#include "systems/ui_system.hpp"

#include "entities/layers.hpp"
#include "player_input.hpp"
#include "brickengine/input_keycode.hpp"
#include "brickengine/json/json.hpp"
#include "scenes/data/level/player_spawn.hpp"
#include "scenes/data/level/gadget_spawn.hpp"
#include "scenes/data/level/solid.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/std/random.hpp"
#include "systems/game_speed_system.hpp"

#include "components/wandering_component.hpp"
#include "scenes/data/level/player_spawn.hpp"
#include "scenes/data/level/gadget_spawn.hpp"
#include "scenes/data/level/solid.hpp"
#include "scenes/main_menu.hpp"
#include "scenes/lobby.hpp"
#include "scenes/help_scene.hpp"
#include "scenes/credits_scene.hpp"

#include "components/stats_component.hpp"

#include "scenes/level_scene.hpp"
#include "scenes/intermission_scene.hpp"
#include "scenes/pause_scene.hpp"
#include "scenes/end_scene.hpp"
#include "scenes/highscore_scene.hpp"
#include "scenes/debug_scene.hpp"
#include "scenes/error_scene.hpp"

#include "data/score.hpp"

GameController::GameController() {
    this->should_quit = false;
    this->should_reset_delta_time = false;
    this->should_draw_fps_counter = false;

    this->delta_time = 1;
#ifdef PERFORMANCE_DEBUGGING
    this->fps_cap = 9999;
#else
    this->fps_cap = 144;
#endif // PERFORMANCE_DEBUGGING

    // From layers.hpp
    this->layers = { 0, 1, 2, 3, 4, 5, 6, 7 };

    this->delta_time_modifier = std::unique_ptr<double>(new double(1));

    engine = std::make_unique<BrickEngine>("Beast Arena", SCREEN_WIDTH, SCREEN_HEIGHT, layers, fps_cap, EntityFactory::FONT_PATH);
    engine->start();
    entityManager = std::make_shared<EntityManager>();
    entityFactory = std::make_shared<EntityFactory>(entityManager, *engine->getRenderableFactory());

    std::unordered_map<std::string, std::set<std::string>> is_trigger_exceptions;
    is_trigger_exceptions.insert({ "Critter", std::set<std::string> { "Platform" } });
    is_trigger_exceptions.insert({ "Weapon", std::set<std::string> { "Platform" } });
    is_trigger_exceptions.insert({ "Player", std::set<std::string> { "Platform" } });
    is_trigger_exceptions.insert({ "Bullet", std::set<std::string> { "Player" } });
    is_trigger_exceptions.insert({ "DeadPlayer", std::set<std::string> { "Platform" } });
    is_trigger_exceptions.insert({ "Spawner", std::set<std::string> { "Platform" } });
    is_trigger_exceptions.insert({ "Ready", std::set<std::string> { "Platform" } });
    is_trigger_exceptions.insert({ "Trophy", std::set<std::string> { "Platform" } });
    collision_detector = std::make_unique<CollisionDetector2>(is_trigger_exceptions, *entityManager);

    createGameStateManager();
    scene_manager = std::make_unique<SceneManager<GameState>>(*entityManager, *game_state_manager);
    setGameStateSystems();
    score_json = std::make_unique<ScoreJson>();
    entityManager->setGetCurrentSceneTagFunction(scene_manager->createGetPrimaryTagFunction());

    setupInput();
    loadMainMenu();
}

void GameController::createGameStateManager(){
    std::unordered_map<GameState, SceneResetState> reset_on_set_state;
    reset_on_set_state.insert({ GameState::InGame, { true, true}});
    reset_on_set_state.insert({ GameState::EndGame, { true, true}});
    reset_on_set_state.insert({ GameState::Highscore, { true, true }});
    reset_on_set_state.insert({ GameState::Menu, { true, true }});
    reset_on_set_state.insert({ GameState::Lobby, { true, true }});
    reset_on_set_state.insert({ GameState::Paused, { false,false }});
    reset_on_set_state.insert({ GameState::LevelDebugger, { true, true }});
    reset_on_set_state.insert({ GameState::Error, { false,false }});

    GameState begin_state = GameState::Unintialized;
    game_state_manager = std::make_unique<GameStateManager<GameState>>(reset_on_set_state, begin_state);
}

void GameController::setGameStateSystems() {
    auto state_systems = std::make_unique<GameStateManager<GameState>::StateSystems>();
    state_systems->insert({ GameState::Menu, std::make_unique<GameStateManager<GameState>::Systems>() });
    state_systems->insert({ GameState::Lobby, std::make_unique<GameStateManager<GameState>::Systems>() });
    state_systems->insert({ GameState::InGame, std::make_unique<std::vector<std::unique_ptr<System>>>() });
    state_systems->insert({ GameState::Paused, std::make_unique<std::vector<std::unique_ptr<System>>>() });
    state_systems->insert({ GameState::EndGame, std::make_unique<std::vector<std::unique_ptr<System>>>() });
    state_systems->insert({ GameState::Highscore, std::make_unique<std::vector<std::unique_ptr<System>>>() });
    state_systems->insert({ GameState::LevelDebugger, std::make_unique<std::vector<std::unique_ptr<System>>>() });
    state_systems->insert({ GameState::Error, std::make_unique<std::vector<std::unique_ptr<System>>>() });

    // Menu
    state_systems->at(GameState::Menu)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::Menu)->push_back(std::make_unique<UISystem>(entityManager, entityFactory, *this));
    state_systems->at(GameState::Menu)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    // Lobby
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<GameSpeedSystem>(entityManager, *delta_time_modifier.get()));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<LobbySystem>(entityFactory, entityManager, *this));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<MovementSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<PhysicsSystem>(*collision_detector, entityManager, *delta_time_modifier.get()));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<PickupSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<CritterSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<WeaponSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<DamageSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<DespawnSystem>(*collision_detector, entityManager, SCREEN_WIDTH, SCREEN_HEIGHT));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<SpawnSystem>(entityManager, entityFactory));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<DisplacementSystem>(*collision_detector, entityManager));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<AnimationSystem>(entityManager));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    // In game
    state_systems->at(GameState::InGame)->push_back(std::make_unique<GameSpeedSystem>(entityManager, *delta_time_modifier.get()));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<CheatSystem>(entityManager, entityFactory, *this));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<GameSystem>(entityManager, *this, *scene_manager));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<PauseSystem>(entityManager, *this));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<MovementSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<PhysicsSystem>(*collision_detector, entityManager, *delta_time_modifier.get()));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<PickupSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<CritterSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<WeaponSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<DamageSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<DespawnSystem>(*collision_detector, entityManager, SCREEN_WIDTH, SCREEN_HEIGHT));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<SpawnSystem>(entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<HUDSystem>(entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<DisplacementSystem>(*collision_detector, entityManager));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<AnimationSystem>(entityManager));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    // Paused
    state_systems->at(GameState::Paused)->push_back(std::make_unique<PauseSystem>(entityManager, *this));
    state_systems->at(GameState::Paused)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::Paused)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    // End game
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<PauseSystem>(entityManager, *this));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<GameSpeedSystem>(entityManager, *delta_time_modifier.get()));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<MovementSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<PhysicsSystem>(*collision_detector, entityManager, *delta_time_modifier.get()));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<PickupSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<CritterSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<ReadyUpSystem>(entityManager, entityFactory, [this]() { this->loadLobby(); }));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<DamageSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<DespawnSystem>(*collision_detector, entityManager, SCREEN_WIDTH, SCREEN_HEIGHT));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<SpawnSystem>(entityManager, entityFactory));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<DisplacementSystem>(*collision_detector, entityManager));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<AnimationSystem>(entityManager));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    // LevelDebugger
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<PauseSystem>(entityManager, *this));
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<DebugSystem>(entityManager, entityFactory, [this](){this->loadDebugger();}));
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<MovementSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<PhysicsSystem>(*collision_detector, entityManager, *delta_time_modifier.get()));
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<PickupSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<CritterSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<WeaponSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<DespawnSystem>(*collision_detector, entityManager, SCREEN_WIDTH, SCREEN_HEIGHT));
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<SpawnSystem>(entityManager, entityFactory));
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<DisplacementSystem>(*collision_detector, entityManager));
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<AnimationSystem>(entityManager));
    state_systems->at(GameState::LevelDebugger)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    // Error
    state_systems->at(GameState::Error)->push_back(std::make_unique<PauseSystem>(entityManager, *this));
    state_systems->at(GameState::Error)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::Error)->push_back(std::make_unique<DebugSystem>(entityManager, entityFactory, [this](){this->loadDebugger();}));
    state_systems->at(GameState::Error)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    // Highscores
    state_systems->at(GameState::Highscore)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::Highscore)->push_back(std::make_unique<HighscoreSystem>(entityManager, entityFactory, *score_json));
    state_systems->at(GameState::Highscore)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    game_state_manager->setStateSystems(std::move(state_systems));
}

void GameController::setupInput() {
    BrickInput<PlayerInput>& input = BrickInput<PlayerInput>::getInstance();
    std::map<int, std::unordered_map<InputKeyCode, PlayerInput>> inputMapping;

    // This map is used to determine what the value of the input should be when pressed.
    std::unordered_map<InputKeyCode, signed int> axis_mapping;

    // Keyboard
    // Player 1
    inputMapping[1][InputKeyCode::EKey_w] = PlayerInput::Y_AXIS;
    inputMapping[1][InputKeyCode::EKey_a] = PlayerInput::X_AXIS;
    inputMapping[1][InputKeyCode::EKey_s] = PlayerInput::Y_AXIS;
    inputMapping[1][InputKeyCode::EKey_d] = PlayerInput::X_AXIS;
    inputMapping[1][InputKeyCode::EKey_q] = PlayerInput::GRAB;
    inputMapping[1][InputKeyCode::EKey_e] = PlayerInput::SHOOT;
    inputMapping[1][InputKeyCode::EKey_mouse_left] = PlayerInput::MOUSE_LEFT;
    inputMapping[1][InputKeyCode::EKey_mouse_right] = PlayerInput::MOUSE_RIGHT;
 
    axis_mapping[InputKeyCode::EKey_w] = 1;
    axis_mapping[InputKeyCode::EKey_a] = -1;
    axis_mapping[InputKeyCode::EKey_s] = -1;
    axis_mapping[InputKeyCode::EKey_d] = 1;

    // Player 2
    inputMapping[2][InputKeyCode::EKey_up] = PlayerInput::Y_AXIS;
    inputMapping[2][InputKeyCode::EKey_left] = PlayerInput::X_AXIS;
    inputMapping[2][InputKeyCode::EKey_down] = PlayerInput::Y_AXIS;
    inputMapping[2][InputKeyCode::EKey_right] = PlayerInput::X_AXIS;
    inputMapping[2][InputKeyCode::EKey_rctrl] = PlayerInput::GRAB;
    inputMapping[2][InputKeyCode::EKey_rshift] = PlayerInput::SHOOT;

    axis_mapping[InputKeyCode::EKey_up] = 1;
    axis_mapping[InputKeyCode::EKey_left] = -1;
    axis_mapping[InputKeyCode::EKey_down] = -1;
    axis_mapping[InputKeyCode::EKey_right] = 1;

    // Player 3
    inputMapping[3][InputKeyCode::EKey_t] = PlayerInput::Y_AXIS;
    inputMapping[3][InputKeyCode::EKey_f] = PlayerInput::X_AXIS;
    inputMapping[3][InputKeyCode::EKey_g] = PlayerInput::Y_AXIS;
    inputMapping[3][InputKeyCode::EKey_h] = PlayerInput::X_AXIS;
    inputMapping[3][InputKeyCode::EKey_r] = PlayerInput::GRAB;
    inputMapping[3][InputKeyCode::EKey_y] = PlayerInput::SHOOT;
    
    axis_mapping[InputKeyCode::EKey_t] = 1;
    axis_mapping[InputKeyCode::EKey_f] = -1;
    axis_mapping[InputKeyCode::EKey_g] = -1;
    axis_mapping[InputKeyCode::EKey_h] = 1;
    
    // Player 4
    inputMapping[4][InputKeyCode::EKey_i] = PlayerInput::Y_AXIS;
    inputMapping[4][InputKeyCode::EKey_j] = PlayerInput::X_AXIS;
    inputMapping[4][InputKeyCode::EKey_k] = PlayerInput::Y_AXIS;
    inputMapping[4][InputKeyCode::EKey_l] = PlayerInput::X_AXIS;
    inputMapping[4][InputKeyCode::EKey_u] = PlayerInput::GRAB;
    inputMapping[4][InputKeyCode::EKey_o] = PlayerInput::SHOOT;

    axis_mapping[InputKeyCode::EKey_i] = 1;
    axis_mapping[InputKeyCode::EKey_j] = -1;
    axis_mapping[InputKeyCode::EKey_k] = -1;
    axis_mapping[InputKeyCode::EKey_l] = 1;

    // Controller
    inputMapping[1][InputKeyCode::EController_x_axis] = PlayerInput::X_AXIS;
    inputMapping[1][InputKeyCode::EController_a] = PlayerInput::Y_AXIS;
    inputMapping[1][InputKeyCode::EController_x] = PlayerInput::SHOOT;
    inputMapping[1][InputKeyCode::EController_b] = PlayerInput::GRAB;
    inputMapping[1][InputKeyCode::EController_start] = PlayerInput::PAUSE;

    inputMapping[2][InputKeyCode::EController_x_axis] = PlayerInput::X_AXIS;
    inputMapping[2][InputKeyCode::EController_a] = PlayerInput::Y_AXIS;
    inputMapping[2][InputKeyCode::EController_x] = PlayerInput::SHOOT;
    inputMapping[2][InputKeyCode::EController_b] = PlayerInput::GRAB;
    inputMapping[2][InputKeyCode::EController_start] = PlayerInput::PAUSE;

    inputMapping[3][InputKeyCode::EController_x_axis] = PlayerInput::X_AXIS;
    inputMapping[3][InputKeyCode::EController_a] = PlayerInput::Y_AXIS;
    inputMapping[3][InputKeyCode::EController_x] = PlayerInput::SHOOT;
    inputMapping[3][InputKeyCode::EController_b] = PlayerInput::GRAB;
    inputMapping[3][InputKeyCode::EController_start] = PlayerInput::PAUSE;

    inputMapping[4][InputKeyCode::EController_x_axis] = PlayerInput::X_AXIS;
    inputMapping[4][InputKeyCode::EController_a] = PlayerInput::Y_AXIS;
    inputMapping[4][InputKeyCode::EController_x] = PlayerInput::SHOOT;
    inputMapping[4][InputKeyCode::EController_b] = PlayerInput::GRAB;
    inputMapping[4][InputKeyCode::EController_start] = PlayerInput::PAUSE;
    
    // Cheats
    inputMapping[20][InputKeyCode::EKey_f1] = PlayerInput::SKIP_LEVEL;
    inputMapping[20][InputKeyCode::EKey_f2] = PlayerInput::KILL_EVERYONE_EXCEPT_YOURSELF;
    inputMapping[20][InputKeyCode::EKey_f3] = PlayerInput::INFINITE_HEALTH;
    inputMapping[20][InputKeyCode::EKey_f4] = PlayerInput::RANDOM_WEAPON;
    inputMapping[20][InputKeyCode::EKey_f5] = PlayerInput::LASER_WEAPON;

    // Debugger
    inputMapping[20][InputKeyCode::EKey_f6] = PlayerInput::REFRESH;

    inputMapping[20][InputKeyCode::EKey_f7] = PlayerInput::TOGGLE_FPS_COUNTER;

    // Pause
    for(int i = 1; i <= 4; ++i) {
        inputMapping[i][InputKeyCode::EController_select] = PlayerInput::QUICK_PLAY;
        inputMapping[i][InputKeyCode::EKey_escape] = PlayerInput::PAUSE;
    }

    // Gamespeed modifier
    inputMapping[20][InputKeyCode::EKey_pageup] = PlayerInput::SPEED_UP;
    inputMapping[20][InputKeyCode::EKey_pagedown] = PlayerInput::SPEED_DOWN;
    inputMapping[20][InputKeyCode::EKey_home] = PlayerInput::SPEED_RESET;

    std::unordered_map<PlayerInput, double> time_to_wait_mapping;
    time_to_wait_mapping[PlayerInput::GRAB] = 0.1;
    time_to_wait_mapping[PlayerInput::MOUSE_LEFT] = 0.1;
    time_to_wait_mapping[PlayerInput::SPEED_DOWN] = 0.1;
    time_to_wait_mapping[PlayerInput::SPEED_UP] = 0.1;
    time_to_wait_mapping[PlayerInput::SPEED_RESET] = 0.1;
    time_to_wait_mapping[PlayerInput::SKIP_LEVEL] = 0.1;
    time_to_wait_mapping[PlayerInput::INFINITE_HEALTH] = 0.1;
    time_to_wait_mapping[PlayerInput::RANDOM_WEAPON] = 0.1;
    time_to_wait_mapping[PlayerInput::KILL_EVERYONE_EXCEPT_YOURSELF] = 0.1;
    time_to_wait_mapping[PlayerInput::LASER_WEAPON] = 0.1;
    time_to_wait_mapping[PlayerInput::TOGGLE_FPS_COUNTER] = 0.1;
    time_to_wait_mapping[PlayerInput::PAUSE] = 0.1;

    input.setInputMapping(inputMapping, time_to_wait_mapping, axis_mapping);
}

void GameController::gameLoop() {
#ifdef PERFORMANCE_DEBUGGING
    double totalTime = 0.0;
    const int FPS_HISTORY_MAX = 500;
    std::deque<int> fps_history { FPS_HISTORY_MAX };
#endif // PERFORMANCE_DEBUGGING

    while(!should_quit) {
        auto start_time = std::chrono::high_resolution_clock::now();

        BrickInput<PlayerInput>::getInstance().processInput(delta_time);

        engine->getRenderer()->clearScreen();

        delta_time *= *delta_time_modifier.get();

        for (auto& system : game_state_manager->getSystems()) {
            system->update(delta_time);
        }
        
        auto& input = BrickInput<PlayerInput>::getInstance();
        if(input.checkInput(20, PlayerInput::TOGGLE_FPS_COUNTER)) 
            should_draw_fps_counter = !should_draw_fps_counter;

        if(should_draw_fps_counter)
            engine->drawFpsCounter();

        engine->getRenderer()->drawScreen();

#ifdef PERFORMANCE_DEBUGGING
        CollisionDetector2Info collision_info = collision_detector->getInfo();
        auto entities_with_colliders = entityManager->getEntitiesByComponent<RectangleColliderComponent>();
        std::cout << "Collision Detector 2 © - continuous calculations: " << collision_info.continuous_calculations_counter << std::endl;
        std::cout << "Collision Detector 2 © - discrete calculations: " << collision_info.discrete_calculated_counter << std::endl;
        std::cout << "FPS: " << engine->getFps() << std::endl;
        std::cout << "Entities with colliders: " << entities_with_colliders.size() << std::endl;
        std::cout << "Total running time: " << totalTime << std::endl;
        int total = 0;
        for (int& fps : fps_history) {
            total += fps;
        }
        std::cout << "Average FPS: " << total / fps_history.size() << std::endl;
#endif // PERFORMANCE_DEBUGGING

        collision_detector->invalidateInfo();

        auto end_time = std::chrono::high_resolution_clock::now();
        engine->delay(start_time, end_time);
        if (should_reset_delta_time) {
            delta_time = 0.00000001;
            should_reset_delta_time = false;
        } else
            delta_time = engine->getDeltatime();

#ifdef PERFORMANCE_DEBUGGING
        totalTime += delta_time;
        if (fps_history.size() >= FPS_HISTORY_MAX)
            fps_history.pop_front();
        fps_history.push_back(engine->getFps());
#endif // PERFORMANCE_DEBUGGING
    }
    engine->getSoundManager().stopMusic();
    engine->stop();
}

SceneManager<GameState>& GameController::getSceneManager() const {
    return *scene_manager.get();
}

int GameController::getScreenWidth() const {
    return SCREEN_WIDTH;
}

int GameController::getScreenHeight() const {
    return SCREEN_HEIGHT;
}

void GameController::loadLobby() {
    scene_manager->destroyAllScenes();
    scene_manager->createScene<Lobby>(*entityFactory, *engine, *this);
    this->should_reset_delta_time = true;
}

void GameController::startGame() {
    auto player_components = entityManager->getEntitiesByComponent<PlayerComponent>();
    if(player_components.size() >= 2) {
        loadLevels();
        loadNextLevel();
    }
}

void GameController::exitGame() {
    should_quit = true;
}

void GameController::loadLevels() {
    // Create list of levels
    std::string levels_path = "assets/levels";
    std::vector<std::string> levels;
    std::vector<std::string> temp_levels;
    for (const auto & entry : std::filesystem::directory_iterator(levels_path))
        levels.push_back(entry.path());

    // Fill queue randomly with levels
    temp_levels = levels;
    auto& r = Random::getInstance();
    while(level_queue.size() != MAX_LEVELS) {
        int random = r.getRandomInt(0, temp_levels.size() - 1);
        level_queue.push(temp_levels.at(random));
        temp_levels.erase(temp_levels.begin() + random);

        // If there are not enough levels
        if(level_queue.size() < MAX_LEVELS && temp_levels.empty()) {
            temp_levels = levels;
        }
    }
}

void GameController::loadNextLevel() {
    // Remove the current scene
    scene_manager->destroyScene(SceneLayer::Primary);
    // Remove the current scene
    if (scene_manager->isSceneActive<IntermissionScene>())
        scene_manager->destroyScene(IntermissionScene::getLayerStatic());

    // Remove the current scene
    if(!level_queue.empty()) {
        // load from queue
        std::string path = level_queue.front();
        level_queue.pop();

        // Create the level
        Json level_json { path, true };
        scene_manager->createScene<LevelScene>(*entityFactory, *engine, level_json);
    } else {
        // There are no levels left in the queue.
        loadEndGameLevel();
    }
    this->should_reset_delta_time = true;
}

void GameController::intermission(int timer) {
    scene_manager->destroyScene(IntermissionScene::getLayerStatic());
    scene_manager->createScene<IntermissionScene>(timer, *entityFactory);
}


void GameController::loadDebugger(){
    scene_manager->destroyAllScenes();
    try{
        Json level_json { "./assets/debugger/level.json", true };

        scene_manager->createScene<DebugScene>(*entityFactory, *engine, level_json);
        this->should_reset_delta_time = true;
    }
    catch(NoValidJsonOrPathException ej){
        scene_manager->createScene<ErrorScene>(*entityFactory, *engine, ej.what(), [this](){this->loadMainMenu();});
    }
    catch(...){
        scene_manager->destroyAllScenes();
        scene_manager->createScene<ErrorScene>(*entityFactory, *engine, "Elements required for the level were not found in your JSON file or in the wrong format.", [this](){this->loadMainMenu();});
    }
}

void GameController::loadMainMenu() {
    scene_manager->destroyAllScenes();
    scene_manager->createScene<MainMenu>(*entityFactory, *engine, *this);
}

void GameController::loadHelp() {
    scene_manager->destroyAllScenes();
    scene_manager->createScene<HelpScene>(*entityFactory, *engine, *this);
}

void GameController::loadCredits() {
    scene_manager->destroyAllScenes();
    scene_manager->createScene<CreditsScene>(*entityFactory, *engine, *this);
}

void GameController::loadEndGameLevel() {
    scene_manager->destroyScene(SceneLayer::Primary);
    // Create scores
    auto players = entityManager->getEntitiesByComponent<PlayerComponent>();
    std::unordered_map<std::string, Score> scores;
    for(auto [entity_id, player] : players) {
        auto stats = entityManager->getComponent<StatsComponent>(entity_id);
        Score score;
        score.accidents = stats->accidents;
        score.deaths = stats->deaths;
        score.killed_critters = stats->killed_critters;
        score.kills = stats->kills;
        score.levels_won = stats->levels_won;
        scores.insert_or_assign(player->name, score);
    }
    score_json->writeScores(scores);
    scene_manager->createScene<EndScene>(*entityFactory, *engine);
    this->should_reset_delta_time = true;
}

void GameController::pauseGame() {
    if(scene_manager->isSceneActive<PauseScene>()){
        scene_manager->destroyScene(PauseScene::getLayerStatic());
        game_state_manager->setState(scene_manager->getLayerState(SceneLayer::Primary));
    }else{
        scene_manager->destroyScene(PauseScene::getLayerStatic());
        scene_manager->createScene<PauseScene>(*entityFactory, *engine, [this](){this->pauseGame();},
                                                [this](){this->loadMainMenu();});
    }
}

void GameController::showHighscores() {
    scene_manager->destroyAllScenes();
    scene_manager->createScene<HighscoreScene>(*entityManager, *entityFactory, *engine, *score_json, *this);
}
