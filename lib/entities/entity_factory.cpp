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
#include "components/character_selection_component.hpp"
#include "components/spawn_component.hpp"
#include "components/hold_component.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/components/data/scale.hpp"
#include "brickengine/components/enums/collision_detection_type.hpp"

EntityFactory::EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf) : entityManager(em), renderableFactory(rf) {
    player_on_death = [em = entityManager](int entity_id) {
        auto transform = em->getComponent<TransformComponent>(entity_id);
        auto player = em->getComponent<PlayerComponent>(entity_id);
        transform->y_direction = Direction::NEGATIVE;
        player->disabled = true;
        em->removeTag(entity_id, "Player");
        em->setTag(entity_id, "DeadPlayer");
        em->addComponentToEntity(entity_id, std::make_unique<PickupComponent>(true, false));
    };
    player_revive = [em = entityManager](int entity_id) {
        auto transform = em->getComponent<TransformComponent>(entity_id);
        auto player = em->getComponent<PlayerComponent>(entity_id);
        auto physics = em->getComponent<PhysicsComponent>(entity_id);
        auto health = em->getComponent<HealthComponent>(entity_id);
        transform->y_direction = Direction::POSITIVE;
        player->disabled = false;
        physics->kinematic = Kinematic::IS_NOT_KINEMATIC;
        health->health = health->max_health;
        em->removeComponentFromEntity<PickupComponent>(entity_id);
        em->removeTag(entity_id, "DeadPlayer");
        em->setTag(entity_id, "Player");
    };
    createPistolComponents = [rf = renderableFactory]() {
        auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto weapon_r = rf.createImage(GRAPHICS_PATH + "weapons/pistol-1.png", (int)Layers::Foreground, std::move(weapon_dst), 255);
        auto bullet_r = rf.createImage(GRAPHICS_PATH + "bullets/1.png", (int)Layers::Middleground, std::move(bullet_dst), 255);
        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

        comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 31, 22, Direction::POSITIVE, Direction::POSITIVE));
        comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
        comps->push_back(std::make_unique<PhysicsComponent>(50, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true, CollisionDetectionType::Discrete));
        comps->push_back(std::make_unique<TextureComponent>(std::move(weapon_r)));
        comps->push_back(std::make_unique<PickupComponent>());
        comps->push_back(std::make_unique<DespawnComponent>(false, true));
        comps->push_back(std::make_unique<WeaponComponent>(
            DamageComponent(25, true),
            TextureComponent(std::move(bullet_r)),
            PhysicsComponent(1, 0, 2250, 0, false, Kinematic::IS_NOT_KINEMATIC, false, false, CollisionDetectionType::Continuous),
            DespawnComponent(true, true),
            RectangleColliderComponent(1, 1, 1, false, false),
            Scale(12, 4),
            0.2, 15));

        std::vector<std::string> tags { "Weapon" };

        return std::make_pair(std::move(comps), tags);
    };
    createRifleComponents = [rf = renderableFactory]() {
        auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto weapon_r = rf.createImage(GRAPHICS_PATH + "weapons/rifle-1.png", (int)Layers::Foreground, std::move(weapon_dst), 255);
        auto bullet_r = rf.createImage(GRAPHICS_PATH + "bullets/1.png", (int)Layers::Middleground, std::move(bullet_dst), 255);
        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

        comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 72, 23, Direction::POSITIVE, Direction::POSITIVE));
        comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
        comps->push_back(std::make_unique<PhysicsComponent>(75, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true, CollisionDetectionType::Discrete));
        comps->push_back(std::make_unique<TextureComponent>(std::move(weapon_r)));
        comps->push_back(std::make_unique<PickupComponent>());
        comps->push_back(std::make_unique<DespawnComponent>(false, true));
        comps->push_back(std::make_unique<WeaponComponent>(
            DamageComponent(50, true),
            TextureComponent(std::move(bullet_r)),
            PhysicsComponent(1, 0, 2500, 0, false, Kinematic::IS_NOT_KINEMATIC, false, false, CollisionDetectionType::Continuous),
            DespawnComponent(true, true),
            RectangleColliderComponent(1, 1, 1, false, false),
            Scale(12, 4),
            0.1, 30));

        std::vector<std::string> tags { "Weapon" };

        return std::make_pair(std::move(comps), tags);
    };
    createSniperComponents = [rf = renderableFactory]() {
        auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto weapon_r = rf.createImage(GRAPHICS_PATH + "weapons/sniper-1.png", (int)Layers::Foreground, std::move(weapon_dst), 255);
        auto bullet_r = rf.createImage(GRAPHICS_PATH + "bullets/1.png", (int)Layers::Middleground, std::move(bullet_dst), 255);
        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

        comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 85, 28, Direction::POSITIVE, Direction::POSITIVE));
        comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
        comps->push_back(std::make_unique<PhysicsComponent>(80, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true, CollisionDetectionType::Discrete));
        comps->push_back(std::make_unique<TextureComponent>(std::move(weapon_r)));
        comps->push_back(std::make_unique<PickupComponent>());
        comps->push_back(std::make_unique<DespawnComponent>(false, true));
        comps->push_back(std::make_unique<WeaponComponent>(
            DamageComponent(50, true),
            TextureComponent(std::move(bullet_r)),
            PhysicsComponent(1, 0, 3000, 0, false, Kinematic::IS_NOT_KINEMATIC, false, false, CollisionDetectionType::Continuous),
            DespawnComponent(true, true),
            RectangleColliderComponent(1, 1, 1, false, false),
            Scale(12, 4),
            0.4, 10));

        std::vector<std::string> tags { "Weapon" };

        return std::make_pair(std::move(comps), tags);
    };
}

int EntityFactory::createPlayer(int player_id, Character character, int x, int y) const {
    auto character_specs = getCharacterSpecs(character);

    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + character_specs.path, (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x, y, character_specs.x_scale, character_specs.y_scale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
    comps->push_back(std::make_unique<PhysicsComponent>(character_specs.mass, true, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false, CollisionDetectionType::Discrete));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(player_id, character_specs.name));
    comps->push_back(std::make_unique<HealthComponent>(character_specs.health, player_on_death, player_revive, POINTS_ON_KILL_PLAYER));
    comps->push_back(std::make_unique<DespawnComponent>(false, false));
    comps->push_back(std::make_unique<HoldComponent>(Position {40, -12}));
    comps->push_back(std::make_unique<StatsComponent>());

    int entity = entityManager->createEntity(std::move(comps), std::nullopt);
    entityManager->setTag(entity, "Player");
    return entity;
}

int EntityFactory::createSpawner(double x_pos, double y_pos, std::vector<GadgetType> gadget_types,
                                 int respawn_timer, bool always_respawn) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + "weapons/spawner-2.png",
                                           (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, 48, 9, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
    comps->push_back(std::make_unique<PhysicsComponent>(100, true, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false, CollisionDetectionType::Discrete));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    std::vector<SpawnComponent::CreateCompsFn> comps_fns;
    for (auto& type : gadget_types) {
        switch (type) {
            case GadgetType::Pistol:
                comps_fns.push_back(createPistolComponents);
                break;
            case GadgetType::Rifle:
                comps_fns.push_back(createRifleComponents);
                break;
            case GadgetType::Sniper:
                comps_fns.push_back(createSniperComponents);
                break;
        }
    }
    comps->push_back(std::make_unique<SpawnComponent>(respawn_timer, comps_fns, always_respawn));

    int entity = entityManager->createEntity(std::move(comps), std::nullopt);
    entityManager->setTag(entity, "Spawner");
    return entity;
}

int EntityFactory::createCritter(double x_pos, double y_pos) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + "beasts/bunny/bunny-1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, 20, 20, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
    comps->push_back(std::make_unique<PhysicsComponent>(50, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false, CollisionDetectionType::Discrete));
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

    int entity_id = entityManager->createEntity(std::move(comps), std::nullopt);
    return entity_id;
}

int EntityFactory::createPlatform(double x_pos, double y_pos, double x_scale, double y_scale, std::string path, int alpha) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + path, (int)Layers::Foreground, std::move(dst), alpha);
 
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, x_scale, y_scale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false, true));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    int entity_id = entityManager->createEntity(std::move(comps), std::nullopt);
    entityManager->setTag(entity_id, "Platform");
    return entity_id;
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

int EntityFactory::createText(std::string text, int x, int y, int x_scale, int y_scale, int font_size, Color color) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto r_text = renderableFactory.createText(text, font_size, color, (int)Layers::UI, std::move(dst));
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x, y, x_scale, y_scale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r_text)));

    return entityManager->createEntity(std::move(comps));
}

int EntityFactory::createCharacterSelector(int player_id, int x, int y) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<CharacterSelectionComponent>(player_id));
    comps->push_back(std::make_unique<TransformComponent>(x, y, 0, 0, Direction::POSITIVE, Direction::POSITIVE));

    return entityManager->createEntity(std::move(comps));
}

void EntityFactory::changeCharacterSelectorTexture(int entity_id, Character character, bool create){
    // When create is true, no texture is being displayed yet
    // When create is false, the old texture needs to be deleted before a new one is added
    if(!create)
        entityManager->removeComponentFromEntity<TextureComponent>(entity_id);
    
    auto character_specs = getCharacterSpecs(character);
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto r = renderableFactory.createImage(GRAPHICS_PATH + character_specs.path, (int)Layers::Middleground, std::move(dst), 255);
    entityManager->addComponentToEntity(entity_id, std::make_unique<TextureComponent>(std::move(r)));

    entityManager->getComponent<TransformComponent>(entity_id)->x_scale = character_specs.x_scale;
    entityManager->getComponent<TransformComponent>(entity_id)->y_scale = character_specs.y_scale;
}

const std::vector<Character> EntityFactory::getAvailableCharacters() const {
    return {
        Character::GORILLA,
        Character::PANDA,
        Character::CHEETAH,
        Character::ELEPHANT
    };
}

const std::vector<std::pair<Character, bool>> EntityFactory::getPickedCharacters() const {
    return {
        std::pair<Character, bool>(Character::GORILLA, false),
        std::pair<Character, bool>(Character::PANDA, false),
        std::pair<Character, bool>(Character::CHEETAH, false),
        std::pair<Character, bool>(Character::ELEPHANT, false),
        std::pair<Character, bool>(Character::RANDOM, false)
    };
}

const CharacterSpecs EntityFactory::getCharacterSpecs(Character character) const {
    CharacterSpecs specs = CharacterSpecs();

    switch(character) {
        case Character::GORILLA:
            specs.path = "beasts/gorilla/gorilla-1.png";
            specs.x_scale = 50;
            specs.y_scale = 100;
            specs.mass = 105;
            specs.name = "Gorilla";
            specs.health = 100;
            break;
        case Character::PANDA:
            specs.path = "beasts/panda/panda-1.png";
            specs.x_scale = 63;
            specs.y_scale = 100;
            specs.mass = 95;
            specs.name = "Panda";
            specs.health = 100;
            break;
        case Character::CHEETAH:
            specs.path = "beasts/cheetah/cheetah-1.png";
            specs.x_scale = 50;
            specs.y_scale = 100;
            specs.mass = 90;
            specs.name = "Cheetah";
            specs.health = 100;
            break;
        case Character::ELEPHANT:
            specs.path = "beasts/elephant/elephant-1.png";
            specs.x_scale = 100;
            specs.y_scale = 100;
            specs.mass = 105;
            specs.name = "Elephant";
            specs.health = 100;
            break;
        case Character::RANDOM:
            specs.path = "menu/question-mark.png";
            specs.x_scale = 50;
            specs.y_scale = 100;
            specs.mass = 100;
            specs.name = "Question-mark";
            specs.health = 100;
            break;
    }
    return specs;
}
