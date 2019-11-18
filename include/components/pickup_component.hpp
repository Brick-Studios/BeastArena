#ifndef PICKUP_COMPONENT_HPP
#define PICKUP_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"

class PickupComponent : public ComponentImpl<PickupComponent>{
public:
    PickupComponent() = default;
    static std::string getNameStatic();
};

#endif // PICKUP_COMPONENT_HPP