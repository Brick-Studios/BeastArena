#include "systems/click_system.hpp"

#include "brickengine/input.hpp"
#include "brickengine/components/click_component.hpp"
#include "brickengine/components/transform_component.hpp"
#include "player_input.hpp"
#include <iostream>

ClickSystem::ClickSystem(std::shared_ptr<EntityManager> em) : System(em) {}

void ClickSystem::update(double) {
    auto input = BrickInput<PlayerInput>::getInstance();

    // Check if the user clicked the left mouse button. If not, return the user out of the method.
    if (!input.checkInput(1, PlayerInput::MOUSE_LEFT)) return;

    // get all click comps
    auto clickComps = entityManager->getEntitiesByComponent<ClickComponent>();

    for (auto& [id, click] : *clickComps) {
        auto transform = entityManager->getComponent<TransformComponent>(id);
        auto [ x, y ] = input.getMousePosition();
        auto xScale = transform->xScale * click->xScale;
        auto yScale = transform->yScale * click->yScale;
        
        double left = transform->xPos - (xScale / 2);
        double right = transform->xPos + (xScale / 2);
        double top = transform->yPos - (yScale / 2);
        double bottom = transform->yPos + (yScale / 2 );
        if (x > left && x < right &&
            y < bottom && y > top) {
            // The button was clicked
            click->fn();
        }
    }
}