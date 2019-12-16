#ifndef FILE_PLAYER_SPAWN_HPP
#define FILE_PLAYER_SPAWN_HPP

#include "brickengine/enum/direction.hpp"

struct PlayerSpawn {
public:
    int x;
    int y;
    Direction direction;
};

#endif // FILE_PLAYER_SPAWN_HPP