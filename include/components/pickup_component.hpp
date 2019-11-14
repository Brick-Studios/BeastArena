#ifndef PICKUP_COMPONENT_HPP
#define PICKUP_COMPONENT_HPP

#include <optional>
#include <chrono>
#include "brickengine/components/component_impl.hpp"

class PickupComponent : public ComponentImpl<PickupComponent>{
public:
    PickupComponent();
    static std::string getNameStatic();

    // Data
    std::optional<std::chrono::high_resolution_clock::time_point> last_interaction;
private:
};

#endif // PICKUP_COMPONENT_HPP