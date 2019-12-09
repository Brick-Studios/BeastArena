#include "entities/entity_factory.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <tuple>

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
#include "components/ready_component.hpp"
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
        em->removeTag(entity_id, "DeadPlayer");
        em->setTag(entity_id, "Player");
    };
    createPistolComponents = [rf = renderableFactory]() {
        auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto weapon_r = rf.createImage(GRAPHICS_PATH + "weapons/pistol-1.png", (int)Layers::Foreground, std::move(weapon_dst), 255);
        auto bullet_r = rf.createImage(GRAPHICS_PATH + "bullets/fire.png", (int)Layers::Middleground, std::move(bullet_dst), 255);
        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

        comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 33, 24, Direction::POSITIVE, Direction::POSITIVE));
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
            Scale(22, 13),
            0.2, 15));

        std::vector<std::string> tags;
        tags.push_back("Weapon");

        return EntityComponents { std::move(comps), tags };
    };
    createRifleComponents = [rf = renderableFactory]() {
        auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto weapon_r = rf.createImage(GRAPHICS_PATH + "weapons/rifle-1.png", (int)Layers::Foreground, std::move(weapon_dst), 255);
        auto bullet_r = rf.createImage(GRAPHICS_PATH + "bullets/droplet.png", (int)Layers::Middleground, std::move(bullet_dst), 255);
        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

        comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 42, 30, Direction::POSITIVE, Direction::POSITIVE));
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
            Scale(14, 7),
            0.1, 30));

        std::vector<std::string> tags;
        tags.push_back("Weapon");

       return EntityComponents { std::move(comps), tags };
    };
    createSniperComponents = [rf = renderableFactory]() {
        auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto weapon_r = rf.createImage(GRAPHICS_PATH + "weapons/sniper-1.png", (int)Layers::Foreground, std::move(weapon_dst), 255);
        auto bullet_r = rf.createImage(GRAPHICS_PATH + "bullets/sniper.png", (int)Layers::Middleground, std::move(bullet_dst), 255);
        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

        comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 96, 28, Direction::POSITIVE, Direction::POSITIVE));
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

        std::vector<std::string> tags;
        tags.push_back("Weapon");

        return EntityComponents { std::move(comps), tags };
    };
}

EntityComponents EntityFactory::createPlayer(int player_id, Character character, int x, int y) const {
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
    comps->push_back(std::make_unique<ReadyComponent>());
    comps->push_back(std::make_unique<PickupComponent>());

    std::vector<std::string> tags;
    tags.push_back("Player");

    return { std::move(comps), tags };
}


EntityComponents EntityFactory::createSpawner(double x_pos, double y_pos, double relative_modifier, std::vector<GadgetType> gadget_types,
                                 int respawn_timer, bool always_respawn) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + "weapons/spawner-2.png",
                                           (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
 
    comps->push_back(std::make_unique<TransformComponent>(x_pos / relative_modifier, y_pos / relative_modifier, 48, 9, Direction::POSITIVE, Direction::POSITIVE));
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

    std::vector<std::string> tags;
    tags.push_back("Spawner");

    return { std::move(comps), tags };
}


EntityComponents EntityFactory::createCritter(double x_pos, double y_pos) const {
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

    std::vector<std::string> tags;
    tags.push_back("Critter");

    return { std::move(comps), tags };
}

EntityComponents EntityFactory::createImage(std::string path, int x_pos, int y_pos, int x_scale, int y_scale, double relative_modifier, Layers layer, int alpha) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + path, (int)layer, std::move(dst), alpha);

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x_pos / relative_modifier, y_pos / relative_modifier, x_scale / relative_modifier, y_scale / relative_modifier, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    std::vector<std::string> tags;

    return { std::move(comps), tags };
}

EntityComponents EntityFactory::createPlatform(double x_pos, double y_pos, double x_scale, double y_scale, double relative_modifier, std::string path, int alpha) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + path, (int)Layers::Foreground, std::move(dst), alpha);
 
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x_pos / relative_modifier, y_pos /relative_modifier, x_scale / relative_modifier, y_scale / relative_modifier, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false, true));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    std::vector<std::string> tags;
    tags.push_back("Platform");
    
    return { std::move(comps), tags };
}

std::vector<EntityComponents> EntityFactory::createButton(std::string text, Color text_color, int font_size,
    std::string texture_path, int x, int y, int x_scale, int y_scale, 
    int alpha, double relative_modifier, std::function<void ()> on_click) {
    std::vector<EntityComponents> component_list;
    {
        // Make background
        auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
        auto r = renderableFactory.createImage(GRAPHICS_PATH + texture_path, (int)Layers::Middleground, std::move(dst), alpha);

        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
        comps->push_back(std::make_unique<TransformComponent>(x / relative_modifier, y / relative_modifier, x_scale / relative_modifier, y_scale / relative_modifier, Direction::POSITIVE, Direction::POSITIVE));
        comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
        comps->push_back(std::make_unique<ClickComponent>(on_click, 1, 1));
        std::vector<std::string> tags;
        component_list.push_back({std::move(comps), tags});
    }
    {
        auto dstText = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
        auto rText = renderableFactory.createText(text, font_size, text_color, (int)Layers::Foreground, std::move(dstText));

        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
        // Relative to button
        int text_x = x / relative_modifier;
        int text_y = (y - 10) / relative_modifier;
        int text_x_scale = ((text.size() * 30) / relative_modifier);
        int text_y_scale = ((y_scale - 30) / relative_modifier);
        comps->push_back(std::make_unique<TransformComponent>(text_x, text_y, text_x_scale, text_y_scale, Direction::POSITIVE, Direction::POSITIVE));
        comps->push_back(std::make_unique<TextureComponent>(std::move(rText)));
        std::vector<std::string> tags;
        component_list.push_back({std::move(comps), tags});
    }
    return std::move(component_list);
}

EntityComponents EntityFactory::createText(std::string text, Color color, int font_size, int x, int y, int x_scale, int y_scale, double relative_modifier) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto r_text = renderableFactory.createText(text, font_size, color, (int)Layers::UI, std::move(dst));
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x / relative_modifier, y / relative_modifier, x_scale / relative_modifier, y_scale / relative_modifier, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r_text)));
    std::vector<std::string> tags;

    return { std::move(comps), tags };
}

EntityComponents EntityFactory::createTrophy(int x, int y, int x_scale, int y_scale, double relative_modifier, Layers layer, int alpha) {
    auto comps = createImage("items/trophy.png", x, y, x_scale, y_scale, relative_modifier, layer, alpha);
    comps.components->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
    comps.components->push_back(std::make_unique<PhysicsComponent>(50, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true, CollisionDetectionType::Discrete));
    comps.components->push_back(std::make_unique<PickupComponent>());
    comps.components->push_back(std::make_unique<DespawnComponent>(false, true));
    comps.tags.push_back("Trophy");
    
    return std::move(comps);
}

int EntityFactory::addToEntityManager(EntityComponents entity_components, std::optional<std::pair<int,bool>> parent_opt, std::optional<std::string> scene_tag) {
    int entity_id = entityManager->createEntity(std::move(entity_components.components), parent_opt, scene_tag);
    for(auto& tag : entity_components.tags) {
        entityManager->setTag(entity_id, tag);
    }
    return entity_id;
}

EntityComponents EntityFactory::createReadySign(int x, int y, int x_scale, int y_scale, double relative_modifier, Layers layer, int alpha) {
    auto comps = createImage("items/ready.png", x, y, x_scale, y_scale, relative_modifier, layer, alpha);
    comps.components->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
    comps.components->push_back(std::make_unique<PhysicsComponent>(50, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true, CollisionDetectionType::Discrete));
    comps.components->push_back(std::make_unique<PickupComponent>());
    comps.components->push_back(std::make_unique<DespawnComponent>(false, true));
    comps.tags.push_back("Ready");

    return { std::move(comps) };
}

EntityComponents EntityFactory::createCharacterSelector(int player_id, int x, int y, double relative_modifier) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<CharacterSelectionComponent>(player_id));
    comps->push_back(std::make_unique<TransformComponent>(x / relative_modifier, y / relative_modifier, 0, 0, Direction::POSITIVE, Direction::POSITIVE));
    std::vector<std::string> tags;

    return { std::move(comps), tags };
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
