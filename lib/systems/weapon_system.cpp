#include "systems/weapon_system.hpp"
#include "components/weapon_component.hpp"
#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "components/despawn_component.hpp"
#include "brickengine/input.hpp"
#include "player_input.hpp"
#include <tuple>

WeaponSystem::WeaponSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void WeaponSystem::update(double deltatime){
    auto& input = BrickInput<PlayerInput>::getInstance();
    auto entities_with_player = entityManager->getEntitiesByComponent<PlayerComponent>();
    auto entities_with_weapon = entityManager->getEntitiesByComponent<WeaponComponent>();

    for (auto& [weapon_id, weapon] : entities_with_weapon) {
        weapon->shoot_cooldown += deltatime;
    }

    for (auto& [entity_id, player]: entities_with_player) {
        if (player->disabled) continue;

        if (input.checkInput(player->player_id, PlayerInput::SHOOT)) {
            // THIS IS AMERICA
            auto children_with_weapon = entityManager->getChildrenWithComponent<WeaponComponent>(entity_id);

            for (auto& [child_id, weapon] : children_with_weapon) {
                if (weapon->shoot_cooldown < weapon->fire_rate)
                    continue;
                weapon->shoot_cooldown = 0;

                auto child_transform = entityManager->getComponent<TransformComponent>(child_id);
                auto [child_absolute_position, child_absolute_scale] = entityManager->getAbsoluteTransform(child_id);
                std::ignore = child_absolute_scale;
                auto bullet_comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

                auto bullet_physics = std::make_unique<PhysicsComponent>(weapon->bullet_physics);
                if (child_transform->x_direction == Direction::NEGATIVE) {
                    bullet_physics->vx *= -1;
                    bullet_physics->vy *= -1;
                }
                if (child_transform->y_direction == Direction::NEGATIVE) {
                    bullet_physics->vx *= -1;
                    bullet_physics->vy *= -1;
                }
                bullet_comps->push_back(std::make_unique<TransformComponent>(
                    child_absolute_position.x, child_absolute_position.y, weapon->bullet_scale.x,
                    weapon->bullet_scale.y, child_transform->x_direction, child_transform->y_direction));
                bullet_comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
                bullet_comps->push_back(std::make_unique<TextureComponent>(weapon->bullet_texture));
                bullet_comps->push_back(std::move(bullet_physics));
                auto damage = std::make_unique<DamageComponent>(weapon->bullet_damage);
                damage->damage_dealer_entity_id = entity_id;
                bullet_comps->push_back(std::move(damage));
                bullet_comps->push_back(std::make_unique<DespawnComponent>(weapon->bullet_despawn));

                entityManager->createEntity(std::move(bullet_comps));

                if (weapon->ammo) {
                    --*weapon->ammo;
                    if (*weapon->ammo <= 0) {
                        entityManager->removeEntity(child_id);
                        // For safety, you should try to use this child after it has been deleted!
                        continue;
                    }
                }
            }
        }
    }
}
