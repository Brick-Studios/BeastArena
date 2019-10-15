#ifndef FILE_BEAST_SYSTEM
#define FILE_BEAST_SYSTEM

#include <memory>
#include "entities/entity_factory.hpp"
#include "brickengine/systems/system.hpp"

class BeastSystem : public System {
public:
    BeastSystem(std::shared_ptr<EntityFactory> ef, std::shared_ptr<EntityManager> _entityManager);
    ~BeastSystem() = default;
private:
    std::shared_ptr<EntityFactory> entityFactory;
};

#endif // FILE_BEAST_SYSTEM
