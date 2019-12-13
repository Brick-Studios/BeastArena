#ifndef FILE_CHARACTER_SPECS_HPP
#define FILE_CHARACTER_SPECS_HPP

#include <string>

struct CharacterSpecs {
public:
    std::string path;
    int x_scale;
    int y_scale;
    int sprite_width;
    int sprite_height;
    int mass;
    std::string name;
    int health;

    std::string mug_texture;
    int mug_x_scale;
    int mug_y_scale;
};

#endif // FILE_CHARACTER_SPECS_HPP