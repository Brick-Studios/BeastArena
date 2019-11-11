#ifndef FILE_SCENE_MANAGER_HPP
#define FILE_SCENE_MANAGER_HPP

#include <memory>
#include <vector>

#include "entities/entity_factory.hpp"
#include "brickengine/entities/entity_manager.hpp"
#include "brickengine/engine.hpp"
#include "level/level.hpp"

class SceneManager {
public:
    SceneManager(std::shared_ptr<EntityFactory> entity_factory, std::shared_ptr<EntityManager> entity_manager, BrickEngine* engine);
    void loadLevel(Level& level);
    void destroyCurrentScene();
private:
    std::shared_ptr<EntityFactory> entity_factory;
    std::shared_ptr<EntityManager> entity_manager;
    BrickEngine* engine;

    std::vector<int> current_scene_entities;
};

#endif // FILE_SCENE_MANAGER_HPP