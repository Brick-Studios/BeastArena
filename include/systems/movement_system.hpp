#ifndef FILE_MOVEMENT_SYSTEM_HPP
#define FILE_MOVEMENT_SYSTEM_HPP

#include <memory>
#include "systems/beast_system.hpp"
#include "entities/entity_factory.hpp"

class MovementSystem : public BeastSystem {
public:
    MovementSystem(std::shared_ptr<EntityManager> entityManager, std::shared_ptr<EntityFactory> ef);
    ~MovementSystem() = default;
    void update(double deltatime);
private:
    static constexpr double TERMINAL_VELOCITY = 500'000;
    static constexpr double JUMP_FORCE = 100'000'000;
    static constexpr double MOVEMENT_FORCE = 5;
};

#endif // FILE_MOVEMENT_SYSTEM_HPP
