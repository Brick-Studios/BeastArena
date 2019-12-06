#ifndef FILE_STATS_COMPONENT_HPP
#define FILE_STATS_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"

class StatsComponent : public ComponentImpl<StatsComponent> {
public:
    static std::string getNameStatic();
    // Data
    int levels_won = 0;
    int kills = 0;
    int killed_critters = 0;
    int deaths = 0;
    int accidents = 0;
};

#endif // FILE_STATS_COMPONENT_HPP