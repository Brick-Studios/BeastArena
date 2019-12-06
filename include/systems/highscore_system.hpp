#ifndef FILE_HIGHSCORE_SYSTEM_HPP
#define FILE_HIGHSCORE_SYSTEM_HPP

#include <map>
#include <array>

#include "jsons/score_json.hpp"
#include "data/score.hpp"
#include "systems/beast_system.hpp"

class HighscoreSystem : public BeastSystem {
public:
    HighscoreSystem(std::shared_ptr<EntityManager> entity_manager, std::shared_ptr<EntityFactory> entity_factory, ScoreJson& score_json);
    void update(double deltatime);
    void reset();
private:
    void createHighscores(std::string name, Score score);
    ScoreJson& score_json;
    std::vector<std::pair<std::string, Score>> scores;
    int selector = 0;
    bool initialized = false;;
    bool empty = false;
};

#endif // FILE_HIGHSCORE_SYSTEM_HPP