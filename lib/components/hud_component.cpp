#include "components/hud_component.hpp"

HUDComponent::HUDComponent(std::string texture, int x_scale, int y_scale)
    : texture(texture), x_scale(x_scale), y_scale(y_scale) {}

std::string HUDComponent::getNameStatic() {
    return "HUDComponent";
}