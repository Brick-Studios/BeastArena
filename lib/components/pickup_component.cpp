#include "components/pickup_component.hpp"

PickupComponent::PickupComponent(bool match_parent_x_direction, bool match_parent_y_direction)
    : match_parent_x_direction(match_parent_x_direction), match_parent_y_direction(match_parent_y_direction) {}

std::string PickupComponent::getNameStatic(){
    return "PickupComponent";
}