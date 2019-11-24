#ifndef FILE_BUTTON_HPP
#define FILE_BUTTON_HPP

#include <string>
#include <functional>

#include "scenes/data/menu/text.hpp"

struct Button {
public:
    std::string texture_path;
    int alpha;

    int x; // Before relative modifier
    int y; // Before relative modifier
    int x_scale; // Before relative modifier
    int y_scale; // Before relative modifier

    Text text;

    std::function<void ()> on_click;
};

#endif // FILE_BUTTON_HPP