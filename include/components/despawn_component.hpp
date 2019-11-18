#ifndef FILE_DESPAWN_COMPONENT_HPP
#define FILE_DESPAWN_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"

class DespawnComponent : public ComponentImpl<DespawnComponent> {
public:
    DespawnComponent(bool despawn_on_collision, bool despawn_on_out_of_screen);
    static std::string getNameStatic();

    // Data
    bool despawn_on_collision;
    bool despawn_on_out_of_screen;
};

#endif // FILE_DESPAWN_COMPONENT_HPP