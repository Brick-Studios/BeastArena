#ifndef FILE_GAME_CONTROLLER_HPP
#define FILE_GAME_CONTROLLER_HPP

#include <memory>
#include <vector>
#include "brickengine/systems/system.hpp"
#include "brickengine/engine.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/rendering/renderable_factory.hpp"
#include "brickengine/entities/entity_manager.hpp"
#include "brickengine/collision_detector.hpp"
#include "entities/entity_factory.hpp"

class GameController {
public:
    GameController();
    void gameLoop();
private:
    void createSystems();
    void setupInput();
    void createTestEntities();

    std::unique_ptr<BrickEngine> engine;
    std::vector<std::unique_ptr<System>> systems;
    std::shared_ptr<EntityManager> entityManager;
    std::shared_ptr<EntityFactory> entityFactory;
    std::shared_ptr<CollisionDetector> collisionDetector;

    std::unique_ptr<Renderable> fps_counter;
    double delta_time;
    int fps_cap;

    std::vector<int> layers;

    inline static const int SCREEN_HEIGHT = 900;
    inline static const int SCREEN_WIDTH = 1600;
};

#endif // FILE_GAME_CONTROLLER_HPP
