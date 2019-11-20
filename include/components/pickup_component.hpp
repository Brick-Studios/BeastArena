#ifndef PICKUP_COMPONENT_HPP
#define PICKUP_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"

class PickupComponent : public ComponentImpl<PickupComponent>{
public:
    PickupComponent(bool match_parent_x_direction = true, bool match_parent_y_direction = true);
    static std::string getNameStatic();
    bool match_parent_x_direction; 
    bool match_parent_y_direction; 
};

#endif // PICKUP_COMPONENT_HPP