#ifndef FILE_SPAWN_COMPONENT_HPP
#define FILE_SPAWN_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"
#include <functional>
#include <memory>
#include <vector>
#include "brickengine/enum/direction.hpp"
#include "enums/gadget_type.hpp"

class SpawnComponent : public ComponentImpl<SpawnComponent> {
public:
    using CreateCompsFn = std::function<std::pair<std::unique_ptr<std::vector<std::unique_ptr<Component>>>, std::vector<std::string>> ()>;

    SpawnComponent(int respawn_timer, std::vector<CreateCompsFn> gadget_fns, bool always_respawn);
    static std::string getNameStatic();

    // Data
    std::vector<CreateCompsFn> comp_fns;
    double respawn_timer;
    double time_elapsed;
    // Always respawn eventhough 
    bool always_respawn;
    std::optional<int> spawned_entity;
};

#endif // FILE_SPAWN_COMPONENT_HPP