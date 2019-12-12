#ifndef FILE_DEBUG_SYSTEM_HPP
#define FILE_DEBUG_SYSTEM_HPP

#include "systems/beast_system.hpp"

class DebugSystem : public BeastSystem {
public:
    DebugSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef,
                std::function<void()> on_reload);
    void update(double delta_time);
private:
    std::function<void()> on_reload;
};

#endif // FILE_DEBUG_SYSTEM_HPP