#ifndef FILE_GADGET_SPAWN_HPP
#define FILE_GADGET_SPAWN_HPP

#include <vector>
#include <string>
#include "enums/gadget_type.hpp"

struct GadgetSpawn {
public:
    std::vector<GadgetType> available_spawns;
    double respawn_timer;
    int x;
    int y;
    bool always_respawn;
};

#endif // FILE_GADGET_SPAWN_HPP