#include <filesystem>

#include "jsons/score_json.hpp"
#include "brickengine/json/json.hpp"
#include "brickengine/components/player_component.hpp"

void ScoreJson::writeScores(std::unordered_map<std::string, Score> scores) {
    Json json;
    if(std::filesystem::exists(HIGHSCORES_PATH)) {
        json = Json { HIGHSCORES_PATH, true };
    }
    for (auto& [name, score] : scores) {
        // Retrieve statistics from the previous game.
        auto kills = score.kills;
        auto deaths = score.deaths;
        auto killed_critters = score.killed_critters; 
        auto levels_won = score.levels_won;
        auto accidents = score.accidents;

        Json object = json.getObject(name);
        if(!object.empty()) {
            kills += object.getInt("kills");
            deaths += object.getInt("deaths");
            killed_critters += object.getInt("killed_critters");
            levels_won += object.getInt("levels_won");
            accidents += object.getInt("accidents");
        }

        // Add the statistics to the json.
        object.setInt("kills", kills);
        object.setInt("deaths", deaths);
        object.setInt("killed_critters", killed_critters);
        object.setInt("levels_won", levels_won);
        object.setInt("accidents", accidents);

        json.setObject(name, object);
    }

    // Write contents to file
    std::ofstream file { HIGHSCORES_PATH };
    file << json;
    file.close();
}

ScoreJson::Scores ScoreJson::readScores() {
    Scores scores;
    if(std::filesystem::exists(HIGHSCORES_PATH)) {
        auto json = Json { HIGHSCORES_PATH, true };
        auto map = json.getUnorderedMap();
        for(auto [key, object] : map) {
            Score score;
            score.accidents = object.getInt("accidents");
            score.kills = object.getInt("kills");
            score.deaths = object.getInt("deaths");
            score.killed_critters = object.getInt("killed_critters");
            score.levels_won = object.getInt("levels_won");
            scores[key] = score;
        }
    }
    return scores;
}