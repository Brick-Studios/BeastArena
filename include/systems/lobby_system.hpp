#ifndef FILE_LOBBY_SYSTEM_HPP
#define FILE_LOBBY_SYSTEM_HPP

#include <vector>
#include <utility>
#include <string>

#include "systems/beast_system.hpp"
#include "entities/entity_factory.hpp"
#include "enums/character.hpp"

class LobbySystem : public BeastSystem {
public:
    LobbySystem(std::shared_ptr<EntityFactory> ef, std::shared_ptr<EntityManager> em);
    void update(double deltatime);
    void reset();
private:
    std::shared_ptr<EntityFactory> ef;
    std::shared_ptr<EntityManager> em;

    std::vector<std::pair<Character, bool>> picked_characters;
    std::vector<std::pair<std::string, bool>> picked_names;
};

#endif // FILE_LOBBY_SYSTEM_HPP