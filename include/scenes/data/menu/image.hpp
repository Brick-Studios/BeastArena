#ifndef FILE_IMAGE_HPP
#define FILE_IMAGE_HPP

#include <string>

#include "entities/layers.hpp"

struct Image {
public:
    std::string texture_path;
    int alpha;

    Layers layer;

    int x; // Before relative modifier
    int y; // Before relative modifier
    int x_scale; // Before relative modifier
    int y_scale; // Before relative modifier
};

#endif // FILE_IMAGE_HPP