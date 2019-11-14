#include "systems/beast_system.hpp"

BeastSystem::BeastSystem(std::shared_ptr<EntityFactory> ef, std::shared_ptr<EntityManager> em)
            : System(em), entity_factory(ef) {}