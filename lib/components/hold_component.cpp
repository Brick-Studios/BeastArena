#include "components/hold_component.hpp"

HoldComponent::HoldComponent(Position position) : position(position) {}

std::string HoldComponent::getNameStatic() {
    return "HoldComponent";
}