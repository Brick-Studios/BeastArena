#ifndef FILE_DAMAGE_COMPONENT_HPP
#define FILE_DAMAGE_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"

class DamageComponent : public ComponentImpl<DamageComponent> {
public:
    DamageComponent(double damage);
    static std::string getNameStatic();
    
    // Data
    double damage;
};

#endif // FILE_DAMAGE_COMPONENT_HPP