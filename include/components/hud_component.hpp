#ifndef FILE_HUD_COMPONENT_HPP
#define FILE_HUD_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"

#include <string>

class HUDComponent : public ComponentImpl<HUDComponent> {
public:
    HUDComponent(std::string texture, int x_scale, int y_scale);
    static std::string getNameStatic();

    // Data
    std::string texture;
    int x_scale;
    int y_scale;

    // Set by the level_scene
    int x;
    int y;
};

#endif // FILE_HUD_COMPONENT_HPP