#include "brickengine/input.hpp"
#include "brickengine/components/player_component.hpp"

#include "systems/pickup_system.hpp"
#include "components/pickup_component.hpp"
#include "components/weapon_component.hpp"
#include "components/hold_component.hpp"
#include "components/wandering_component.hpp"
#include "player_input.hpp"

PickupSystem::PickupSystem(CollisionDetector2& cd,
    std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void PickupSystem::update(double){    
    auto& input = BrickInput<PlayerInput>::getInstance();
    auto entities_with_player = entityManager->getEntitiesByComponent<PlayerComponent>();

    for (auto& [entity_id, player]: entities_with_player) {
        if (player->disabled) continue;
        auto hold_comp = entityManager->getComponent<HoldComponent>(entity_id);
        if (!hold_comp) continue;

        if (input.checkInput(player->player_id, PlayerInput::GRAB)) {
            auto children = entityManager->getChildren(entity_id);
            if (!children.empty()) {
                for (const int& child : children) {
                    auto child_pickup = entityManager->getComponent<PickupComponent>(child);
                    if(child_pickup) {
                        entityManager->moveOutOfParentsHouse(child);
                        auto pickup_collider = entityManager->getComponent<RectangleColliderComponent>(child);
                        pickup_collider->should_displace = true;
                    }
                }
            }
            else {
                auto collisions = collision_detector.detectDiscreteCollision(entity_id);
                auto entity_transform = entityManager->getComponent<TransformComponent>(entity_id);
                auto [ entity_position, entity_scale ] = entityManager->getAbsoluteTransform(entity_id);

                for (auto& collision : collisions) {
                    if (!collision.is_trigger) continue;
                    int pickup_entity_id = collision.opposite_id;
                    auto pickup = entityManager->getComponent<PickupComponent>(pickup_entity_id);
                    if (!pickup) continue;

                    auto pickup_collider = entityManager->getComponent<RectangleColliderComponent>(pickup_entity_id);
                    pickup_collider->should_displace = false;

                    auto pickup_transform = entityManager->getComponent<TransformComponent>(pickup_entity_id);
                    entityManager->moveOutOfParentsHouse(collision.opposite_id);

                    // Set position
                    if (entity_transform->x_direction == Direction::NEGATIVE) {
                        pickup_transform->x_pos = hold_comp->position.x * -1;
                    } else {
                        pickup_transform->x_pos = hold_comp->position.x;
                    }
                    if (entity_transform->y_direction == Direction::NEGATIVE) {
                        pickup_transform->y_pos = hold_comp->position.y * -1;
                    } else {
                        pickup_transform->y_pos = hold_comp->position.y;
                    }

                    // Set direction
                    if (pickup->match_parent_x_direction)
                        pickup_transform->x_direction = entity_transform->x_direction;
                    if (pickup->match_parent_y_direction)
                        pickup_transform->y_direction = entity_transform->y_direction;

                    // Set scale
                    pickup_transform->x_scale /= entity_scale.x;
                    pickup_transform->y_scale /= entity_scale.y;

                    entityManager->setParent(pickup_entity_id, entity_id, true);

                    auto child_wander = entityManager->getComponent<WanderingComponent>(pickup_entity_id);
                    if (child_wander)
                        child_wander->killer = entity_id;

                    // We can only pick up one single thing, so now that we have picked up something, break the loop
                    break;
                }
            }
        }
    }
}
