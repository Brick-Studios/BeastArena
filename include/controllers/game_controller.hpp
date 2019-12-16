#ifndef FILE_GAME_CONTROLLER_HPP
#define FILE_GAME_CONTROLLER_HPP

#include <memory>
#include <vector>
#include "brickengine/systems/system.hpp"
#include "brickengine/engine.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/rendering/renderable_factory.hpp"
#include "brickengine/entities/entity_manager.hpp"
#include "brickengine/collision_detector_2.hpp"
#include "brickengine/scenes/scene_manager.hpp"
#include "brickengine/game_state_manager.hpp"
#include "entities/entity_factory.hpp"
#include "enums/game_state.hpp"
#include "jsons/score_json.hpp"

class GameController {
public:
    GameController();
    void gameLoop();

    SceneManager<GameState>& getSceneManager() const;
    int getScreenWidth() const;
    int getScreenHeight() const;
    void loadLobby();
    void startGame();
    void exitGame();
    void loadLevels();
    void loadNextLevel();
    void loadMainMenu();
    void loadHelp();
    void loadCredits();
    void loadEndGameLevel();
    void pauseGame();
    void showHighscores();
    void intermission(int timer);
    void loadDebugger();

    static constexpr int MAX_LEVELS = 10;
    inline static const int SCREEN_HEIGHT = 900;
    inline static const int SCREEN_WIDTH = 1600;
private:
    bool should_quit;
    bool should_reset_delta_time;
    bool should_draw_fps_counter;

    void createGameStateManager();
    void setGameStateSystems();
    void setupInput();

    std::unique_ptr<BrickEngine> engine;
    std::shared_ptr<EntityManager> entityManager;
    std::shared_ptr<EntityFactory> entityFactory;
    std::unique_ptr<CollisionDetector2> collision_detector;
    std::unique_ptr<SceneManager<GameState>> scene_manager;
    std::unique_ptr<GameStateManager<GameState>> game_state_manager;
    std::unique_ptr<ScoreJson> score_json;

    std::unique_ptr<Renderable> fps_counter;
    double delta_time;
    int fps_cap;
    std::unique_ptr<double> delta_time_modifier;

    std::queue<std::string> level_queue;

    std::vector<int> layers;
};

#endif // FILE_GAME_CONTROLLER_HPP
