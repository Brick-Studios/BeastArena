#include "entities/entity_factory.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"

#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/components/physics_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"

EntityFactory::EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf) : entityManager(em), renderableFactory(rf) {}

int EntityFactory::createPanda(double x, double y, int playerId) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/panda/idle-1.png", (int)Layers::Foreground, std::move(dst));
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x, y, 63, 100));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1));
    comps->push_back(std::make_unique<PhysicsComponent>(100, 0, 0, 0, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(playerId));

    return entityManager->createEntity(std::move(comps));
}

int EntityFactory::createGorilla(double x, double y, int playerId) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/gorilla/idle-1.png", (int)Layers::Foreground, std::move(dst));
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x, y, 50, 100));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1));
    comps->push_back(std::make_unique<PhysicsComponent>(100, 0, 0, 0, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(playerId));

    return entityManager->createEntity(std::move(comps));
}

int EntityFactory::createImage(std::string path, int x, int y, int width, int height, Layers layer) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + path, (int)layer, std::move(dst));

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x, y, width, height));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    return entityManager->createEntity(std::move(comps));
}


int EntityFactory::createPlatform(double x, double y, double xScale, double yScale) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "black.jpg", (int)Layers::Foreground, std::move(dst));

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x, y, xScale, yScale));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1));
    comps->push_back(std::make_unique<PhysicsComponent>(130, 0, 0, 0, false, true));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    return entityManager->createEntity(std::move(comps));
}
