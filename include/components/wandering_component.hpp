#ifndef FILE_WANDERING_COMPONENT_HPP
#define FILE_WANDERING_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"
#include <functional>
#include <optional>
#include "brickengine/enum/direction.hpp"

class WanderingComponent : public ComponentImpl<WanderingComponent> {
public:
    WanderingComponent();
    static std::string getNameStatic();

    // Data
    double duration = 1;
    double elapsed_time = 0;
    double waited_for = 0;
    double wait_duration = 10;
    Direction direction;
    bool can_move = false;
    std::optional<int> killer = std::nullopt;
};

#endif // FILE_WANDERING_COMPONENT_HPP
