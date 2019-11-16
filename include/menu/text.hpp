#ifndef FILE_TEXT_HPP
#define FILE_TEXT_HPP

#include <string>

#include "brickengine/rendering/renderables/data/color.hpp"

struct Text {
public:
    std::string text;
    int font_size;
    Color color;

    int x; // Before relative modifier
    int y; // Before relative modifier
    int x_scale; // Before relative modifier
    int y_scale; // Before relative modifier
};

#endif // FILE_TEXT_HPP