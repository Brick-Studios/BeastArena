#include "systems/critter_system.hpp"
#include "components/wandering_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/std/random.hpp"
#include <tuple>

CritterSystem::CritterSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void CritterSystem::update(double deltatime){
    auto& r = Random::getInstance();
    auto entities_with_wandering = entityManager->getEntitiesByComponent<WanderingComponent>();

    for (auto& [entity_id, wander]: entities_with_wandering) {
        auto physics = entityManager->getComponent<PhysicsComponent>(entity_id);
        if (!physics) continue;

        double vx = physics->vx;
        double vy = physics->vy;
        double mass = physics->mass;

        // Delay before moving again
        if (wander->waited_for < wander->wait_duration / 10 && !wander->can_move) {
            wander->waited_for += deltatime;
        }
        // Reset variables
        else if (!wander->can_move){
            wander->can_move = true;
            wander->elapsed_time = 0;
            wander->waited_for = 0;
            wander->duration = r.getRandomInt(0, 20);
            auto random = r.getRandomInt(0, 1);
            if (random == 1) {
                wander->direction = Direction::POSITIVE;
            } else {
                wander->direction = Direction::NEGATIVE;
            }
            // If critter is in front of a wall and is trying to go that direction use opposite direction
            if (wander->direction == Direction::NEGATIVE) {
                auto right = collision_detector->spaceLeft(entity_id, Axis::X, Direction::NEGATIVE);
                if (right.space_left * -1 <= 0) {
                    wander->direction = Direction::POSITIVE;
                }
            } else {
                auto left = collision_detector->spaceLeft(entity_id, Axis::X, Direction::POSITIVE);
                if(left.space_left <= 0){
                    wander->direction = Direction::NEGATIVE;
                }
            }
        }

        // Keep moving while below duration time
        if(wander->elapsed_time < wander->duration / 10 && wander->can_move) {
            wander->elapsed_time += deltatime;

            // Moving right
            if (wander->direction == Direction::POSITIVE) {
                    if (vx < 0) vx = 0;
                    vx += TERMINAL_VELOCITY * MOVEMENT_FORCE / mass;
                    if (vx > TERMINAL_VELOCITY / mass) {
                        vx = TERMINAL_VELOCITY / mass;
                    }
                    // If there is a entity in front of the critter start jumping
                    auto left = collision_detector->spaceLeft(entity_id, Axis::X, Direction::POSITIVE);
                    if (left.space_left <= 0) {
                        bool on_platform = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::POSITIVE).space_left == 0;

                        if (on_platform) {
                            vy = -1 * (JUMP_FORCE / mass);
                        }                    
                    }
            // Moving left
            } else {
                    if (vx > 0) vx = 0;
                    vx += -1 * TERMINAL_VELOCITY * MOVEMENT_FORCE / mass;
                    if (vx < (TERMINAL_VELOCITY * -1) / mass) {
                        vx = TERMINAL_VELOCITY * -1 / mass;
                    }
                    // If there is a entity in front of the critter start jumping
                    auto right = collision_detector->spaceLeft(entity_id, Axis::X, Direction::NEGATIVE);
                    if (right.space_left <= 0) {
                        bool on_platform = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::POSITIVE).space_left == 0;

                        if (on_platform) {
                            vy = -1 * (JUMP_FORCE / mass);
                        }                    
                    }

            }
            physics->vx = vx + r.getRandomInt(1, 5);
            physics->vy = vy;
        } 
        // If time runs out reset and give a random wait duration
        else if (wander->can_move) {
            wander->can_move = false;
            wander->wait_duration = r.getRandomInt(10, 20);
            wander->waited_for = 0;
        }
    }
}
