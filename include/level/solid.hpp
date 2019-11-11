#ifndef FILE_SOLID_HPP
#define FILE_SOLID_HPP

#include <string>
#include "brickengine/rendering/renderables/data/color.hpp"

enum SolidShape { RECTANGLE };
enum SolidEffect { NONE };

struct Solid {
public:
    SolidShape shape;
    SolidEffect effect;

    std::string texture_path;
    int alpha; // Transparancy

    int x; // Before relative modifier
    int y; // Before relative modifier
    int xScale; // Before relative modifier
    int yScale; // Before relative modifier
};

#endif // FILE_SOLID_HPP