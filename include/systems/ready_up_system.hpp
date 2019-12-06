#ifndef FILE_READY_UP_SYSTEM_HPP
#define FILE_READY_UP_SYSTEM_HPP

#include <functional>

#include "systems/beast_system.hpp"

class ReadyUpSystem : public BeastSystem {
public:
    ReadyUpSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef, std::function<void()> ready_function);
    void update(double delta_time) override;
    void reset() override;
private:
    std::function<void()> ready_function;
    bool everyone_is_ready = false;
    double timer = 0;
    std::set<int> player_ready;
};

#endif // FILE_READY_UP_SYSTEM_HPP