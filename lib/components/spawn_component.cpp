#include "components/spawn_component.hpp"

SpawnComponent::SpawnComponent(int respawn_timer, std::vector<CreateCompsFn> comp_fns, bool always_respawn) 
                : respawn_timer(respawn_timer), time_elapsed(respawn_timer), comp_fns(comp_fns),
                  always_respawn(always_respawn), spawned_entity(std::nullopt) {}

std::string SpawnComponent::getNameStatic() {
    return "SpawnComponent";
}