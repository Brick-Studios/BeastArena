#include <iostream>
#include <tuple>

#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/physics_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/input.hpp"

#include "systems/movement_system.hpp"
#include "player_input.hpp"

MovementSystem::MovementSystem(std::shared_ptr<EntityManager> entityManager, std::shared_ptr<EntityFactory> ef) : BeastSystem(ef, entityManager) {}

void MovementSystem::update(double deltatime) {
    auto input = BrickInput<PlayerInput>::getInstance();
    auto entitiesWithPlayer = entityManager->getEntitiesByComponent<PlayerComponent>();

     for (auto& [entityId, player]: *entitiesWithPlayer) {
        std::ignore = player;
        auto physics = entityManager->getComponent<PhysicsComponent>(entityId);
        if (!physics) continue;

        //Input::getInstance().getPlayerInput(player.playerId).checkInput(BeastArenaInput::Right)

        double vx = physics->vx;
        double vy = physics->vy;
        double mass = physics->mass;

        // Moving left or right
        if (input.checkInput(PlayerInput::PLAYER1_LEFT)) {
            if (vx > 0) vx = 0;
            vx += -1 * TERMINAL_VELOCITY * MOVEMENT_FORCE / mass * deltatime;
            if (vx < (TERMINAL_VELOCITY * -1) / mass) {
                vx = (TERMINAL_VELOCITY * -1) / mass;
            }
        } else {
            if (input.checkInput(PlayerInput::PLAYER1_RIGHT)) {
                if (vx < 0) vx = 0;
                vx += TERMINAL_VELOCITY * MOVEMENT_FORCE / mass * deltatime;
                if (vx > TERMINAL_VELOCITY / mass) {
                    vx = TERMINAL_VELOCITY / mass;
                }
            } else {
                vx = 0;
            }
        }
        // Jumping
        if (input.checkInput(PlayerInput::PLAYER1_UP)) {
            // also check you are standing on a platform
            if (vy == 0)
                vy = -1 * (JUMP_FORCE / mass) * deltatime;
        }

        physics->vx = vx;
        physics->vy = vy;
    }
}