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
#include "components/stats_component.hpp"
#include "components/wandering_component.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/components/data/scale.hpp"

EntityFactory::EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf) : entityManager(em), renderableFactory(rf) {
    player_on_death = [em = entityManager](int entity_id) {
        auto transform = em->getComponent<TransformComponent>(entity_id);
        transform->y_direction = Direction::NEGATIVE;
        auto player = em->getComponent<PlayerComponent>(entity_id);
        player->disabled = true;
        auto collider = em->getComponent<RectangleColliderComponent>(entity_id);
        collider->is_trigger = true;
        em->addComponentToEntity(entity_id, std::make_unique<PickupComponent>(true, false));
    };
    player_revive = [em = entityManager](int entity_id) {
        auto transform = em->getComponent<TransformComponent>(entity_id);
        transform->y_direction = Direction::POSITIVE;
        auto player = em->getComponent<PlayerComponent>(entity_id);
        player->disabled = false;
        auto collider = em->getComponent<RectangleColliderComponent>(entity_id);
        collider->is_trigger = false;
        em->removeComponentFromEntity<PickupComponent>(entity_id);
        auto physics = em->getComponent<PhysicsComponent>(entity_id);
        physics->kinematic = Kinematic::IS_NOT_KINEMATIC;
        auto health = em->getComponent<HealthComponent>(entity_id);
        health->health = health->max_health;
    };
}

int EntityFactory::createGorilla(int player_id) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + "beasts/gorilla/gorilla-1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 50, 100, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<PhysicsComponent>(105, true, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(player_id, "Gorilla"));
    comps->push_back(std::make_unique<HealthComponent>(100, player_on_death, player_revive, POINTS_ON_KILL_PLAYER));
    comps->push_back(std::make_unique<DespawnComponent>(false, false));
    comps->push_back(std::make_unique<StatsComponent>());

    int entity = entityManager->createEntity(std::move(comps), std::nullopt);
    entityManager->setTag(entity, "Player");
    return entity;
}

int EntityFactory::createPanda(int player_id) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + "beasts/panda/panda-1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 63, 100, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<PhysicsComponent>(95, true, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(player_id, "Panda"));
    comps->push_back(std::make_unique<HealthComponent>(100, player_on_death, player_revive, POINTS_ON_KILL_PLAYER));
    comps->push_back(std::make_unique<DespawnComponent>(false, false));
    comps->push_back(std::make_unique<StatsComponent>());

    int entity = entityManager->createEntity(std::move(comps), std::nullopt);
    entityManager->setTag(entity, "Player");
    return entity;
}

int EntityFactory::createCheetah(int player_id) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + "beasts/cheetah/cheetah-1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 50, 100, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<PhysicsComponent>(90, true, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(player_id, "Cheetah"));
    comps->push_back(std::make_unique<HealthComponent>(100, player_on_death, player_revive, POINTS_ON_KILL_PLAYER));
    comps->push_back(std::make_unique<DespawnComponent>(false, false));
    comps->push_back(std::make_unique<StatsComponent>());

    int entity = entityManager->createEntity(std::move(comps), std::nullopt);
    entityManager->setTag(entity, "Player");
    return entity;
}

int EntityFactory::createElephant(int player_id) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + "beasts/elephant/elephant-1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 100, 100, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<PhysicsComponent>(105, true, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(player_id, "Elephant"));
    comps->push_back(std::make_unique<HealthComponent>(100, player_on_death, player_revive, POINTS_ON_KILL_PLAYER));
    comps->push_back(std::make_unique<DespawnComponent>(false, false));
    comps->push_back(std::make_unique<StatsComponent>());

    int entity = entityManager->createEntity(std::move(comps), std::nullopt);
    entityManager->setTag(entity, "Player");
    return entity;
}

int EntityFactory::createPistol(double x_pos, double y_pos, bool ammo) const {
    auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto weapon_r = renderableFactory.createImage(GRAPHICS_PATH + "weapons/pistol-1.png", (int)Layers::Foreground, std::move(weapon_dst), 255);
    auto bullet_r = renderableFactory.createImage(GRAPHICS_PATH + "bullets/1.png", (int)Layers::Middleground, std::move(bullet_dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    std::optional<int> ammoOpt;
    if (ammo) ammoOpt = 15;

    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, 31, 22, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true));
    comps->push_back(std::make_unique<PhysicsComponent>(50, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true));
    comps->push_back(std::make_unique<TextureComponent>(std::move(weapon_r)));
    comps->push_back(std::make_unique<PickupComponent>());
    comps->push_back(std::make_unique<WeaponComponent>(
        DamageComponent(25),
        TextureComponent(std::move(bullet_r)),
        PhysicsComponent(1, 0, 2250, 0, false, Kinematic::IS_NOT_KINEMATIC, false, false),
        DespawnComponent(true, true),
        Scale(12, 4),
        0.2, ammoOpt));

    int entity = entityManager->createEntity(std::move(comps), std::nullopt);
    entityManager->setTag(entity, "Weapon");
    return entity;
}

int EntityFactory::createRifle(double x_pos, double y_pos, bool ammo) const {
    auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto weapon_r = renderableFactory.createImage(GRAPHICS_PATH + "weapons/rifle-1.png", (int)Layers::Foreground, std::move(weapon_dst), 255);
    auto bullet_r = renderableFactory.createImage(GRAPHICS_PATH + "bullets/1.png", (int)Layers::Middleground, std::move(bullet_dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    std::optional<int> ammoOpt;
    if (ammo) ammoOpt = 30;

    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, 71, 23, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true));
    comps->push_back(std::make_unique<PhysicsComponent>(75, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true));
    comps->push_back(std::make_unique<TextureComponent>(std::move(weapon_r)));
    comps->push_back(std::make_unique<PickupComponent>());
    comps->push_back(std::make_unique<WeaponComponent>(
        DamageComponent(50),
        TextureComponent(std::move(bullet_r)),
        PhysicsComponent(1, 0, 2500, 0, false, Kinematic::IS_NOT_KINEMATIC, false, false),
        DespawnComponent(true, true),
        Scale(12, 4),
        0.1, ammoOpt));

    int entity = entityManager->createEntity(std::move(comps), std::nullopt);
    entityManager->setTag(entity, "Weapon");
    return entity;
}

int EntityFactory::createSniper(double x_pos, double y_pos, bool ammo) const {
    auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto weapon_r = renderableFactory.createImage(GRAPHICS_PATH + "weapons/sniper-1.png", (int)Layers::Foreground, std::move(weapon_dst), 255);
    auto bullet_r = renderableFactory.createImage(GRAPHICS_PATH + "bullets/1.png", (int)Layers::Middleground, std::move(bullet_dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    std::optional<int> ammoOpt;
    if (ammo) ammoOpt = 10;

    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, 85, 28, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true));
    comps->push_back(std::make_unique<PhysicsComponent>(80, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true));
    comps->push_back(std::make_unique<TextureComponent>(std::move(weapon_r)));
    comps->push_back(std::make_unique<PickupComponent>());
    comps->push_back(std::make_unique<WeaponComponent>(
        DamageComponent(50),
        TextureComponent(std::move(bullet_r)),
        PhysicsComponent(1, 0, 3000, 0, false, Kinematic::IS_NOT_KINEMATIC, false, false),
        DespawnComponent(true, true),
        Scale(12, 4),
        0.4, ammoOpt));

    int entity = entityManager->createEntity(std::move(comps), std::nullopt);
    entityManager->setTag(entity, "Weapon");
    return entity;
}

int EntityFactory::createCritter(double x_pos, double y_pos) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + "beasts/bunny/bunny-1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, 20, 20, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true));
    comps->push_back(std::make_unique<PhysicsComponent>(50, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<WanderingComponent>());
    comps->push_back(std::make_unique<DespawnComponent>(false, true));
    comps->push_back(std::make_unique<PickupComponent>());
    comps->push_back(std::make_unique<HealthComponent>(1, [em = entityManager](int entity_id) {
        em->removeEntity(entity_id);
    }));

    int entity = entityManager->createEntity(std::move(comps), std::nullopt);
    entityManager->setTag(entity, "Critter");
    return entity;
}

int EntityFactory::createImage(std::string path, int x_pos, int y_pos, int x_scale, int y_scale, Layers layer, int alpha) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + path, (int)layer, std::move(dst), alpha);

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, x_scale, y_scale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createPlatform(double x_pos, double y_pos, double x_scale, double y_scale, std::string path, int alpha) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + path, (int)Layers::Foreground, std::move(dst), alpha);
 
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, x_scale, y_scale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

std::pair<int, int> EntityFactory::createButton(const Button button, const double relative_modifier) {
    // Make background
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto r = renderableFactory.createImage(GRAPHICS_PATH + button.texture_path, (int)Layers::Middleground, std::move(dst), button.alpha);

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

int EntityFactory::createText(std::string text, int x, int y, int x_scale, int y_scale) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto r_text = renderableFactory.createText(text, 50, { 0, 255, 0, 255 }, (int)Layers::UI, std::move(dst));
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x, y, x_scale, y_scale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r_text)));

    return entityManager->createEntity(std::move(comps));
}
