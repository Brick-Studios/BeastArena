#include "systems/click_system.hpp"

#include "brickengine/input.hpp"
#include "brickengine/components/click_component.hpp"
#include "brickengine/components/transform_component.hpp"
#include "player_input.hpp"
#include <iostream>

ClickSystem::ClickSystem(std::shared_ptr<EntityManager> em) : System(em) {}

void ClickSystem::update(double) {
    auto input = BrickInput<PlayerInput>::getInstance();

    // Here we do not check if the player is disabled and always use player one, because ClickComponents are not used for gameplay
    // Check if the user clicked the left mouse button. If not, return the user out of the method.
    if (!input.checkInput(1, PlayerInput::MOUSE_LEFT)) return;

    // Get all click comps
    auto clickComps = entityManager->getEntitiesByComponent<ClickComponent>();

    for (auto& [id, click] : clickComps) {
        auto [ position, scale ] = entityManager->getAbsoluteTransform(id);
        auto [ x, y ] = input.getMousePosition();
        auto xScale = scale.x * click->xScale;
        auto yScale = scale.y * click->yScale;
        
        double left = position.x - (xScale / 2);
        double right = position.x + (xScale / 2);
        double top = position.y - (yScale / 2);
        double bottom = position.y + (yScale / 2 );
        if (x > left && x < right &&
            y < bottom && y > top) {
            // The button was clicked
            click->fn();
        }
    }
}