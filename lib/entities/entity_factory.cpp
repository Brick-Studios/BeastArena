#include "entities/entity_factory.hpp"

#include <string>
#include <utility>

#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/components/physics_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/components/click_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"
#include "components/pickup_component.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"

EntityFactory::EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf) : entityManager(em), renderableFactory(rf) {}

int EntityFactory::createGorilla(double xPos, double yPos, int playerId) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/gorilla/idle-1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(xPos, yPos, 50, 100, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<PhysicsComponent>(105, 0, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(playerId));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createPanda(double xPos, double yPos, int playerId) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/panda/idle-1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(xPos, yPos, 63, 100, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<PhysicsComponent>(100, 0, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(playerId));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createWeapon(double xPos, double yPos) const{
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "weapons/pistol-1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(xPos, yPos, 22, 31, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true));
    comps->push_back(std::make_unique<PhysicsComponent>(50, 0, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PickupComponent>());
    //comps->push_back(std::make_unique<WeaponComponent>(1, 1, 10, 5));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createImage(std::string path, int xPos, int yPos, int xScale, int yScale, Layers layer, int alpha) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + path, (int)layer, std::move(dst), alpha);

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(xPos, yPos, xScale, yScale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}


int EntityFactory::createPlatform(double xPos, double yPos, double xScale, double yScale, std::string path, int alpha) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + path, (int)Layers::Foreground, std::move(dst), alpha);
 
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(xPos, yPos, xScale, yScale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

std::pair<int, int> EntityFactory::createButton(const Button button, const double relative_modifier) {
    // Make background
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto r = renderableFactory.createImage(graphicsPath + button.texture_path, (int)Layers::Middleground, std::move(dst), button.alpha);

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(button.x / relative_modifier, button.y / relative_modifier, button.x_scale / relative_modifier, button.y_scale / relative_modifier, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<ClickComponent>(button.on_click, 1, 1));

    int button_id = entityManager->createEntity(std::move(comps), std::nullopt);

    auto dstText = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto rText = renderableFactory.createText(button.text.text, button.text.font_size, button.text.color, (int)Layers::Foreground, std::move(dstText));

    auto compsText = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    int x = button.text.x / relative_modifier;
    int y = button.text.y / relative_modifier;
    int xScale = (button.text.x_scale / relative_modifier) / 1.5;
    int yScale = (button.text.y_scale / relative_modifier) / 1.5;
    compsText->push_back(std::make_unique<TransformComponent>(x, y, xScale, yScale, Direction::POSITIVE, Direction::POSITIVE));
    compsText->push_back(std::make_unique<TextureComponent>(std::move(rText)));

    int text_id = entityManager->createEntity(std::move(compsText), std::nullopt);

    return std::make_pair(button_id, text_id);
}
