#ifndef FILE_LEVEL_HPP
#define FILE_LEVEL_HPP

#include <string>
#include <vector>

#include "scenes/scene.hpp"
#include "brickengine/json/json.hpp"
#include "level/player_spawn.hpp"
#include "level/gadget_spawn.hpp"
#include "level/solid.hpp"

class Level : public Scene {
public:
    Level(Json json, int screen_width, int screen_height);

    // General information
    std::string description;

    // Player spawns
    std::vector<PlayerSpawn> player_spawns;

    // Gadget spawns
    std::vector<GadgetSpawn> gadget_spawns;

    // Solids (floors and walls)
    std::vector<Solid> solids;
};

#endif // FILE_LEVEL_HPP