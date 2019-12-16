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
#include "components/name_selection_component.hpp"
#include "components/spawn_component.hpp"
#include "components/hold_component.hpp"
#include "components/hud_component.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/components/animation_component.hpp"
#include "brickengine/components/data/scale.hpp"

EntityFactory::EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf) : entityManager(em), renderableFactory(rf) {
    player_on_death = [em = entityManager](int entity_id) {
        auto transform = em->getComponent<TransformComponent>(entity_id);
        auto player = em->getComponent<PlayerComponent>(entity_id);
        auto animation = em->getComponent<AnimationComponent>(entity_id);
        auto physics = em->getComponent<PhysicsComponent>(entity_id);
        if (animation) {
            animation->sprite_size = 1;
        }
        transform->y_direction = Direction::NEGATIVE;
        player->disabled = true;
        physics->vx = 0;
        em->removeTag(entity_id, "Player");
        em->setTag(entity_id, "DeadPlayer");
        em->addComponentToEntity(entity_id, std::make_unique<PickupComponent>(true, false));
    };
    player_revive = [em = entityManager](int entity_id) {
        auto transform = em->getComponent<TransformComponent>(entity_id);
        auto player = em->getComponent<PlayerComponent>(entity_id);
        auto physics = em->getComponent<PhysicsComponent>(entity_id);
        auto health = em->getComponent<HealthComponent>(entity_id);
        auto animation = em->getComponent<AnimationComponent>(entity_id);
        auto collider = em->getComponent<RectangleColliderComponent>(entity_id);
        if (animation) {
            // Hardcoded to 2 until we actually make a good animation system
            animation->sprite_size = 2;
        }
        transform->y_direction = Direction::POSITIVE;
        player->disabled = false;
        physics->kinematic = Kinematic::IS_NOT_KINEMATIC;
        health->health = health->max_health;
        collider->should_displace = true;
        em->removeTag(entity_id, "DeadPlayer");
        em->setTag(entity_id, "Player");
        em->removeComponentFromEntity<PickupComponent>(entity_id);
    };
    createPistolComponents = [rf = renderableFactory]() {
        auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto weapon_r = rf.createImage(GRAPHICS_PATH + "weapons/pistol-1.png", (int)Layers::Gadgets, std::move(weapon_dst), 255);
        auto bullet_r = rf.createImage(GRAPHICS_PATH + "bullets/fire.png", (int)Layers::Foreground, std::move(bullet_dst), 255);
        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

        comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 26, 19, Direction::POSITIVE, Direction::POSITIVE));
        comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
        comps->push_back(std::make_unique<PhysicsComponent>(50, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true, CollisionDetectionType(true, false)));
        comps->push_back(std::make_unique<TextureComponent>(std::move(weapon_r)));
        comps->push_back(std::make_unique<PickupComponent>());
        comps->push_back(std::make_unique<DespawnComponent>(false, true));
        comps->push_back(std::make_unique<WeaponComponent>(
            DamageComponent(60, true),
            TextureComponent(std::move(bullet_r)),
            PhysicsComponent(1, 0, 2250, 0, false, Kinematic::IS_NOT_KINEMATIC, false, false, CollisionDetectionType(true, true)),
            DespawnComponent(true, true),
            RectangleColliderComponent(1, 1, 1, false, false),
            Scale(18, 10),
            0.8, 15));

        std::vector<std::string> tags;
        tags.push_back("Weapon");

        return EntityComponents { std::move(comps), tags };
    };
    createRifleComponents = [rf = renderableFactory]() {
        auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto weapon_r = rf.createImage(GRAPHICS_PATH + "weapons/rifle-1.png", (int)Layers::Gadgets, std::move(weapon_dst), 255);
        auto bullet_r = rf.createImage(GRAPHICS_PATH + "bullets/droplet.png", (int)Layers::Foreground, std::move(bullet_dst), 255);
        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

        comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 33, 24, Direction::POSITIVE, Direction::POSITIVE));
        comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
        comps->push_back(std::make_unique<PhysicsComponent>(75, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true, CollisionDetectionType(true, false)));
        comps->push_back(std::make_unique<TextureComponent>(std::move(weapon_r)));
        comps->push_back(std::make_unique<PickupComponent>());
        comps->push_back(std::make_unique<DespawnComponent>(false, true));
        comps->push_back(std::make_unique<WeaponComponent>(
            DamageComponent(20, true),
            TextureComponent(std::move(bullet_r)),
            PhysicsComponent(1, 0, 2500, 0, false, Kinematic::IS_NOT_KINEMATIC, false, false, CollisionDetectionType(true, true)),
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
        auto weapon_r = rf.createImage(GRAPHICS_PATH + "weapons/sniper-1.png", (int)Layers::Gadgets, std::move(weapon_dst), 255);
        auto bullet_r = rf.createImage(GRAPHICS_PATH + "bullets/sniper.png", (int)Layers::Foreground, std::move(bullet_dst), 255);
        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

        comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 76, 22, Direction::POSITIVE, Direction::POSITIVE));
        comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
        comps->push_back(std::make_unique<PhysicsComponent>(80, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true, CollisionDetectionType(true, false)));
        comps->push_back(std::make_unique<TextureComponent>(std::move(weapon_r)));
        comps->push_back(std::make_unique<PickupComponent>());
        comps->push_back(std::make_unique<DespawnComponent>(false, true));
        comps->push_back(std::make_unique<WeaponComponent>(
            DamageComponent(200, true),
            TextureComponent(std::move(bullet_r)),
            PhysicsComponent(1, 0, 3000, 0, false, Kinematic::IS_NOT_KINEMATIC, false, false, CollisionDetectionType(true, true)),
            DespawnComponent(true, true),
            RectangleColliderComponent(1, 1, 1, false, false),
            Scale(12, 4),
            1, 10));

        std::vector<std::string> tags;
        tags.push_back("Weapon");

        return EntityComponents { std::move(comps), tags };
    };
    createLaserComponents = [rf = renderableFactory]() {
        auto weapon_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto bullet_dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
        auto weapon_r = rf.createImage(GRAPHICS_PATH + "weapons/banana-1.png", (int)Layers::Gadgets, std::move(weapon_dst), 255);
        auto bullet_r = rf.createImage(GRAPHICS_PATH + "bullets/banana-bullet-1.png", (int)Layers::Foreground, std::move(bullet_dst), 255);
        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

        comps->push_back(std::make_unique<TransformComponent>(-2000, -2000, 70, 50, Direction::POSITIVE, Direction::POSITIVE));
        comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
        comps->push_back(std::make_unique<PhysicsComponent>(80, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true, CollisionDetectionType(true, false)));
        comps->push_back(std::make_unique<TextureComponent>(std::move(weapon_r)));
        comps->push_back(std::make_unique<PickupComponent>());
        comps->push_back(std::make_unique<DespawnComponent>(false, true));
        comps->push_back(std::make_unique<WeaponComponent>(
            DamageComponent(100, true),
            TextureComponent(std::move(bullet_r)),
            PhysicsComponent(1, 0, 3000, 0, false, Kinematic::IS_NOT_KINEMATIC, false, false, CollisionDetectionType(true, true)),
            DespawnComponent(true, true),
            RectangleColliderComponent(1, 1, 1, false, false),
            Scale(102, 12),
            0.1, 9999));

        std::vector<std::string> tags;
        tags.push_back("Weapon");

        return EntityComponents { std::move(comps), tags };
    };
}

EntityComponents EntityFactory::createPlayer(int player_id, Character character, std::string name, int x, int y) const {
    auto character_specs = getCharacterSpecs(character);

    auto src = std::unique_ptr<Rect>(new Rect{ 0, 0, character_specs.sprite_width, 32 });
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + character_specs.path, (int)Layers::Foreground, std::move(dst), std::move(src), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x, y, character_specs.x_scale, character_specs.y_scale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
    comps->push_back(std::make_unique<PhysicsComponent>(character_specs.mass, true, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false, CollisionDetectionType(true, false)));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(player_id, name));
    comps->push_back(std::make_unique<HealthComponent>(character_specs.health, player_on_death, player_revive, POINTS_ON_KILL_PLAYER));
    comps->push_back(std::make_unique<DespawnComponent>(false, false));
    comps->push_back(std::make_unique<HoldComponent>(Position { character_specs.hold_x, character_specs.hold_y }));
    comps->push_back(std::make_unique<StatsComponent>());
    comps->push_back(std::make_unique<ReadyComponent>());
    comps->push_back(std::make_unique<HUDComponent>(character_specs.mug_texture, character_specs.mug_x_scale, character_specs.mug_y_scale));
    comps->push_back(std::make_unique<AnimationComponent>(0.2, 2));

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
 
    comps->push_back(std::make_unique<TransformComponent>(x_pos / relative_modifier, y_pos / relative_modifier, 38, 7, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
    comps->push_back(std::make_unique<PhysicsComponent>(100, true, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false, CollisionDetectionType(true, false)));
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
    auto src = std::unique_ptr<Rect>(new Rect{ 0, 0, 14, 13 });
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + "beasts/bunny/bunny-idle.png", (int)Layers::Foreground, std::move(dst), std::move(src), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x_pos, y_pos, 20, 20, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
    comps->push_back(std::make_unique<PhysicsComponent>(50, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false, CollisionDetectionType(true, false)));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<WanderingComponent>());
    comps->push_back(std::make_unique<DespawnComponent>(false, true));
    comps->push_back(std::make_unique<PickupComponent>());
    comps->push_back(std::make_unique<AnimationComponent>(0.2, 2));

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

EntityComponents EntityFactory::createImage(std::string path, int x_pos, int y_pos, int x_scale, int y_scale, 
                                            double relative_modifier, Layers layer, int alpha, int sprite_width, 
                                            int sprite_height, double update_time, int sprite_size) {
    auto src = std::unique_ptr<Rect>(new Rect{ 0, 0, sprite_width, sprite_height });
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(GRAPHICS_PATH + path, (int)layer, std::move(dst), std::move(src), alpha);

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x_pos / relative_modifier, y_pos / relative_modifier, x_scale / relative_modifier, y_scale / relative_modifier, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<AnimationComponent>(update_time, sprite_size));

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
        auto r = renderableFactory.createImage(GRAPHICS_PATH + texture_path, (int)Layers::UI, std::move(dst), alpha);

        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
        comps->push_back(std::make_unique<TransformComponent>(x / relative_modifier, y / relative_modifier, x_scale / relative_modifier, y_scale / relative_modifier, Direction::POSITIVE, Direction::POSITIVE));
        comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
        comps->push_back(std::make_unique<ClickComponent>(on_click, 1, 1));
        std::vector<std::string> tags;
        component_list.push_back({std::move(comps), tags});
    }
    {
        auto dstText = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
        auto rText = renderableFactory.createText(FONT_PATH, text, font_size, text_color, (int)Layers::UIForeground, std::move(dstText));

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

EntityComponents EntityFactory::createText(std::string text, Color color, int font_size, int x, int y, int x_scale, int y_scale, double relative_modifier, Layers layer) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto r_text = renderableFactory.createText(FONT_PATH, text, font_size, color,  (int)layer, std::move(dst));
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x / relative_modifier, y / relative_modifier, x_scale / relative_modifier, y_scale / relative_modifier, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r_text)));
    std::vector<std::string> tags;

    return { std::move(comps), tags };
}

EntityComponents EntityFactory::createTrophy(int x, int y, int x_scale, int y_scale, double relative_modifier, Layers layer, int alpha) {
    auto comps = createImage("items/trophy.png", x, y, x_scale, y_scale, relative_modifier, layer, alpha);
    comps.components->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true, true));
    comps.components->push_back(std::make_unique<PhysicsComponent>(50, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true, CollisionDetectionType(true, false)));
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
    comps.components->push_back(std::make_unique<PhysicsComponent>(50, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true, CollisionDetectionType(true, false)));
    comps.components->push_back(std::make_unique<PickupComponent>());
    comps.components->push_back(std::make_unique<DespawnComponent>(false, true));
    comps.tags.push_back("Ready");

    return { std::move(comps) };
}

EntityComponents EntityFactory::createWeaponDrop() {
    auto comps = createRifleComponents();
    return { std::move(comps) };
}

EntityComponents EntityFactory::createLaser() {
    auto comps = createLaserComponents();
    return { std::move(comps) };
}

EntityComponents EntityFactory::createCharacterSelector(int player_id, int x, int y, double relative_modifier, int left_arrow_id, int right_arrow_id) {
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<CharacterSelectionComponent>(player_id, left_arrow_id, right_arrow_id));
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
    auto src = std::unique_ptr<Rect>(new Rect{ 0, 0 , character_specs.sprite_width, character_specs.sprite_height});
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto r = renderableFactory.createImage(GRAPHICS_PATH + character_specs.path, (int)Layers::Middleground, std::move(dst), std::move(src), 255);
    entityManager->addComponentToEntity(entity_id, std::make_unique<TextureComponent>(std::move(r)));
    entityManager->addComponentToEntity(entity_id, std::make_unique<AnimationComponent>(0,1));

    entityManager->getComponent<TransformComponent>(entity_id)->x_scale = character_specs.x_scale;
    entityManager->getComponent<TransformComponent>(entity_id)->y_scale = character_specs.y_scale;
}

EntityComponents EntityFactory::createNameSelector(int player_id, int x, int y, double relative_modifier, int left_arrow_id, int right_arrow_id) {
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<NameSelectionComponent>(player_id, left_arrow_id, right_arrow_id));
    comps->push_back(std::make_unique<TransformComponent>(x / relative_modifier, y / relative_modifier, 0, 0, Direction::POSITIVE, Direction::POSITIVE));
    std::vector<std::string> tags;

    return { std::move(comps), tags };
}

void EntityFactory::changeNameSelectorName(int entity_id, std::string name, bool create, bool final){
    // When create is true, no name is being displayed yet
    // When create is false, the old name needs to be deleted before a new one is added
    if(!create)
        entityManager->removeComponentFromEntity<TextureComponent>(entity_id);

    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});

    if(name.size() > 20) {
        // This is too long --> break it
        name = name.substr(0, 17) + "...";
    }

    Color color;
    if(final) {
        color = { 10, 176, 0, 255 };
    } else {
        color = { 255, 255, 255, 255 };
    }

    auto r_text = renderableFactory.createText(FONT_PATH, name, 72, color,  (int)Layers::Middleground, std::move(dst));
    entityManager->addComponentToEntity(entity_id, std::make_unique<TextureComponent>(std::move(r_text)));

    // We move out of the parents house, set the transform and move back in.
    // So that the entityManager can calculate the relativity for us.
    int parent = *entityManager->getParent(entity_id);
    entityManager->moveOutOfParentsHouse(entity_id);

    int x_scale;
    if(name.size() <= 5) {
        // Scale needs to be resized to prevent deforming
        x_scale = name.size() * 30;
    } else {
        x_scale = 200;
    }

    entityManager->getComponent<TransformComponent>(entity_id)->x_scale = x_scale;
    entityManager->getComponent<TransformComponent>(entity_id)->y_scale = 40;
    entityManager->setParent(entity_id, parent, false);
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
            specs.path = "beasts/gorilla/gorilla-idle.png";
            specs.x_scale = 40;
            specs.y_scale = 80;
            specs.sprite_width = 16;
            specs.sprite_height = 32;
            specs.mass = 100;
            specs.name = "Gorilla";
            specs.health = 120;
            specs.mug_texture = "mugshots/gorilla.png";
            specs.mug_x_scale = 78;
            specs.mug_y_scale = 66;
            specs.hold_x = 32;
            specs.hold_y = -9;
            break;
        case Character::PANDA:
            specs.path = "beasts/panda/panda-idle.png";
            specs.x_scale = 50;
            specs.y_scale = 80;
            specs.sprite_width = 20;
            specs.sprite_height = 32;
            specs.mass = 95;
            specs.name = "Panda";
            specs.health = 100;
            specs.mug_texture = "mugshots/panda.png";
            specs.mug_x_scale = 96;
            specs.mug_y_scale = 66;
            specs.hold_x = 35;
            specs.hold_y = -14;
            break;
        case Character::CHEETAH:
            specs.path = "beasts/cheetah/cheetah-idle.png";
            specs.x_scale = 40;
            specs.y_scale = 80;
            specs.sprite_width = 17;
            specs.sprite_height = 32;
            specs.mass = 90;
            specs.name = "Cheetah";
            specs.health = 70;
            specs.mug_texture = "mugshots/cheetah.png";
            specs.mug_x_scale = 72;
            specs.mug_y_scale = 60;
            specs.hold_x = 27;
            specs.hold_y = -10;
            break;
        case Character::ELEPHANT:
            specs.path = "beasts/elephant/elephant-idle.png";
            specs.x_scale = 80;
            specs.y_scale = 80;
            specs.sprite_width = 32;
            specs.sprite_height = 32;
            specs.mass = 105;
            specs.name = "Elephant";
            specs.health = 160;
            specs.mug_texture = "mugshots/elephant.png";
            specs.mug_x_scale = 140;
            specs.mug_y_scale = 68;
            specs.hold_x = 45;
            specs.hold_y = -25;
            break;
        case Character::RANDOM:
            specs.path = "menu/question-mark-idle.png";
            specs.x_scale = 40;
            specs.y_scale = 80;
            specs.sprite_width = 16;
            specs.sprite_height = 32;
            specs.mass = 100;
            specs.name = "Question-mark";
            specs.health = 100;
            specs.mug_texture = "";
            specs.mug_x_scale = 0;
            specs.mug_y_scale = 0;
            specs.hold_x = 30;
            specs.hold_y = -10;
            break;
    }
    return specs;
}
