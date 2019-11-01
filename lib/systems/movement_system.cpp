#include <iostream>
#include <tuple>

#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/physics_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/input.hpp"

#include "systems/movement_system.hpp"
#include "player_input.hpp"

MovementSystem::MovementSystem(std::shared_ptr<CollisionDetector> cd,
    std::shared_ptr<EntityManager> entityManager, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, entityManager), collisionDetector(cd) {}

void MovementSystem::update(double deltatime) {
    auto input = BrickInput<PlayerInput>::getInstance();
    auto entitiesWithPlayer = entityManager->getEntitiesByComponent<PlayerComponent>();

     for (auto& [entityId, player]: *entitiesWithPlayer) {
        std::ignore = player;
        auto physics = entityManager->getComponent<PhysicsComponent>(entityId);
        if (!physics) continue;

        double vx = physics->vx;
        double vy = physics->vy;
        double mass = physics->mass;

        // Moving left or right
        if (input.checkInput(player->playerId, PlayerInput::LEFT)) {
            if (vx > 0) vx = 0;
            vx += -1 * TERMINAL_VELOCITY * MOVEMENT_FORCE / mass;
            if (vx < (TERMINAL_VELOCITY * -1) / mass) {
                vx = (TERMINAL_VELOCITY * -1) / mass;
            }
        } else {
        if (input.checkInput(player->playerId, PlayerInput::RIGHT)) {
            if (vx < 0) vx = 0;
                vx += TERMINAL_VELOCITY * MOVEMENT_FORCE / mass;
                if (vx > TERMINAL_VELOCITY / mass) {
                    vx = TERMINAL_VELOCITY / mass;
                }
            } else {
                vx = 0;
            }
        }
        // Jumping
        if (input.checkInput(player->playerId, PlayerInput::UP)) {
            bool standsOnPlatform = collisionDetector->spaceLeft(entityId, Axis::Y, Direction::POSITIVE) == 0;

            if (standsOnPlatform) {
                vy = -1 * (JUMP_FORCE / mass);
            }
        }

        physics->vx = vx;
        physics->vy = vy;
    }
}