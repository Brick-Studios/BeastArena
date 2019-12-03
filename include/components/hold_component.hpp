#ifndef FILE_HOLD_COMPONENT_HPP
#define FILE_HOLD_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"
#include "brickengine/components/data/position.hpp"
#include "enums/gadget_type.hpp"
#include <unordered_map>

class HoldComponent : public ComponentImpl<HoldComponent> {
public:
    HoldComponent(Position position);
    static std::string getNameStatic();

    // Data
    // These values should ALWAYS be in the positive direction.
    // If the entity is facing the negative direction whilst the grabbing occurs
    // the position will be made negative for you.
    Position position;
};

#endif // FILE_HOLD_COMPONENT_HPP