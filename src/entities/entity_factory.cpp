#include "entities/entity_factory.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"

#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/physics_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"
#include "entities/layers.hpp"

EntityFactory::EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf) : entityManager(em), renderableFactory(rf) {}

int EntityFactory::createPanda(double x, double y) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/panda/idle-1.png", (int)Layers::Foreground, std::move(dst));
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x, y, 200, 200));
    comps->push_back(std::make_unique<PhysicsComponent>(100, 0, 0, 0, false, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>());

    return entityManager->createEntity(std::move(comps));
}

int EntityFactory::createGorilla(double x, double y) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/gorilla/idle-1.png", (int)Layers::Foreground, std::move(dst));
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x, y, 200, 200));
    comps->push_back(std::make_unique<PhysicsComponent>(130, 0, 0, 0, false, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>());

    return entityManager->createEntity(std::move(comps));
}

int EntityFactory::createImage(std::string path, int x, int y, int width, int heigth) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + path, (int)Layers::Foreground, std::move(dst));

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x, y, width, heigth));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    return entityManager->createEntity(std::move(comps));
}
