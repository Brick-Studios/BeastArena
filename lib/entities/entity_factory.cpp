#include "entities/entity_factory.hpp"

#include <iostream>
#include <string>
#include <utility>

#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/components/physics_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/components/click_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"
#include "components/pickup_component.hpp"
#include "components/health_component.hpp"
#include "components/damage_component.hpp"
#include "components/weapon_component.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/components/data/scale.hpp"

EntityFactory::EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf) : entityManager(em), renderableFactory(rf) {}

int EntityFactory::createGorilla(double x_pos, double y_pos, int player_id) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/gorilla/idle-1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, 50, 100, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<PhysicsComponent>(105, 0, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(player_id));
    comps->push_back(std::make_unique<HealthComponent>(100, [](int entity_id) {

    }));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createPanda1(double x_pos, double y_pos, int player_id) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/panda/1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, 63, 100, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<PhysicsComponent>(100, 0, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(player_id));
    comps->push_back(std::make_unique<ClickComponent>([]() -> void {
        std::cout << "clicked" << std::endl;
    }, 1, 1));
    comps->push_back(std::make_unique<HealthComponent>(100, [em = entityManager](int entity_id) {
        auto transform = em->getComponent<TransformComponent>(entity_id);
        transform->y_direction = Direction::NEGATIVE;
        auto player = em->getComponent<PlayerComponent>(entity_id);
        player->disabled = true;
    }));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createPanda2(double x_pos, double y_pos, int player_id) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/panda/2.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, 63, 100, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<PhysicsComponent>(100, 0, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(player_id));
    comps->push_back(std::make_unique<ClickComponent>([]() -> void {
        std::cout << "clicked" << std::endl;
    }, 1, 1));
    comps->push_back(std::make_unique<HealthComponent>(100, [em = entityManager](int entity_id) {
        auto transform = em->getComponent<TransformComponent>(entity_id);
        transform->y_direction = Direction::NEGATIVE;
        auto player = em->getComponent<PlayerComponent>(entity_id);
        player->disabled = true;
    }));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createPanda3(double x_pos, double y_pos, int player_id) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/panda/3.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, 63, 100, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<PhysicsComponent>(100, 0, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(player_id));
    comps->push_back(std::make_unique<ClickComponent>([]() -> void {
        std::cout << "clicked" << std::endl;
    }, 1, 1));
    comps->push_back(std::make_unique<HealthComponent>(100, [em = entityManager](int entity_id) {
        auto transform = em->getComponent<TransformComponent>(entity_id);
        transform->y_direction = Direction::NEGATIVE;
        auto player = em->getComponent<PlayerComponent>(entity_id);
        player->disabled = true;
    }));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createWeapon(double x_pos, double y_pos, bool ammo) const {
    auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto weapon_r = renderableFactory.createImage(graphicsPath + "weapons/pistol-1.png", (int)Layers::Foreground, std::move(weapon_dst), 255);
    auto bullet_r = renderableFactory.createImage(graphicsPath + "bullets/1.png", (int)Layers::Middleground, std::move(bullet_dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    std::optional<int> ammoOpt;
    if (ammo) ammoOpt = 15;

    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, 31, 22, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true));
    comps->push_back(std::make_unique<PhysicsComponent>(50, 0, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true));
    comps->push_back(std::make_unique<TextureComponent>(std::move(weapon_r)));
    comps->push_back(std::make_unique<PickupComponent>());
    comps->push_back(std::make_unique<WeaponComponent>(
        DamageComponent(10),
        TextureComponent(std::move(bullet_r)),
        PhysicsComponent(1, 0, 4500, 0, false, Kinematic::IS_NOT_KINEMATIC, false, false),
        DespawnComponent(true, true),
        Scale(9, 3),
        0.2, ammoOpt));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createImage(std::string path, int x_pos, int y_pos, int x_scale, int y_scale, Layers layer, int alpha) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + path, (int)layer, std::move(dst), alpha);

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, x_scale, y_scale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createPlatform(double x_pos, double y_pos, double x_scale, double y_scale, std::string path, int alpha) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + path, (int)Layers::Foreground, std::move(dst), alpha);
 
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, x_scale, y_scale, Direction::POSITIVE, Direction::POSITIVE));
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
    int x_scale = (button.text.x_scale / relative_modifier) / 1.5;
    int y_scale = (button.text.y_scale / relative_modifier) / 1.5;
    compsText->push_back(std::make_unique<TransformComponent>(x, y, x_scale, y_scale, Direction::POSITIVE, Direction::POSITIVE));
    compsText->push_back(std::make_unique<TextureComponent>(std::move(rText)));

    int text_id = entityManager->createEntity(std::move(compsText), std::nullopt);

    return std::make_pair(button_id, text_id);
}
