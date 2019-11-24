#ifndef FILE_DAMAGE_COMPONENT_HPP
#define FILE_DAMAGE_COMPONENT_HPP

#include <optional>
#include "brickengine/components/component_impl.hpp"

class DamageComponent : public ComponentImpl<DamageComponent> {
public:
    DamageComponent(double damage, std::optional<int> damage_dealer_entity_id = std::nullopt);
    static std::string getNameStatic();
    
    // Data
    double damage;
    std::optional<int> damage_dealer_entity_id;
};

#endif // FILE_DAMAGE_COMPONENT_HPP