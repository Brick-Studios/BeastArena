#ifndef FILE_SCENE_HPP
#define FILE_SCENE_HPP

#include <string>

#include "brickengine/json/json.hpp"

class Scene {
public:
    Scene(Json json, int screen_width, int screen_height);
    Scene(int screen_width, int screen_height, int width, int height);

    // General information
    double version;
    std::string name;
    double relative_modifier; // Modifier needed to create entities at the right place with the right size
    std::string bg_path;
    std::string bg_music;
private:
    void calculateRelativeModifier(int screen_width, int screen_height, int width, int height);
};

#endif // FILE_SCENE_HPP