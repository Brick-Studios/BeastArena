#include <iostream>
#include <tuple>
#include <math.h>

#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/physics_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/input.hpp"
#include "brickengine/std/floating_point_comparer.hpp"

#include "systems/movement_system.hpp"
#include "player_input.hpp"

MovementSystem::MovementSystem(CollisionDetector2& cd,
    std::shared_ptr<EntityManager> entityManager, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, entityManager), collision_detector(cd) {}

void MovementSystem::update(double) {
    auto& input = BrickInput<PlayerInput>::getInstance();
    auto entitiesWithPlayer = entityManager->getEntitiesByComponent<PlayerComponent>();

     for (auto& [entity_id, player]: entitiesWithPlayer) {
        if (player->disabled) continue;

        auto physics = entityManager->getComponent<PhysicsComponent>(entity_id);
        if (!physics || (physics && physics->kinematic == Kinematic::IS_KINEMATIC)) continue;

        double vx = physics->vx;
        double vy = physics->vy;
        double mass = physics->mass;

        // Moving left or right
        auto x_movement = input.checkInput(player->player_id, PlayerInput::X_AXIS);
        if (x_movement < 0) {
            if (vx > 0) 
                vx = 0;
            vx = x_movement * MOVEMENT_FORCE / mass;
        }
        else if (x_movement > 0) {
            if (vx < 0) 
                vx = 0;
            vx = x_movement * MOVEMENT_FORCE / mass;
        } else {
            vx = 0;
        }
        // Jumping
        if (input.checkInput(player->player_id, PlayerInput::Y_AXIS) == 1) {
            auto collision = collision_detector.detectContinuousCollision(entity_id, Axis::Y, Direction::POSITIVE);
            bool is_on_platform = FloatingPointComparer::is_equal_to_zero(collision.space_left);

            if (is_on_platform) {
                vy = -1 * (JUMP_FORCE / mass);
            }
        }

        physics->vx = vx;
        physics->vy = vy;
    }
}
