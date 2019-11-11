#ifndef FILE_SCENE_HPP
#define FILE_SCENE_HPP

#include <string>

class Scene {
public:
    // General information
    double version;
    std::string name;
    double relative_modifier; // Modifier needed to create entities at the right place with the right size
    std::string bg_path;
    std::string bg_music;
};

#endif // FILE_SCENE_HPP