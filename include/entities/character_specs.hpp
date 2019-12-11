#ifndef FILE_CHARACTER_SPECS_HPP
#define FILE_CHARACTER_SPECS_HPP

#include <string>

struct CharacterSpecs {
public:
    std::string path;
    int x_scale;
    int y_scale;
    int sprite_width;
    int mass;
    std::string name;
    int health;
};

#endif // FILE_CHARACTER_SPECS_HPP