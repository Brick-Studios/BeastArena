#include "scenes/scene.hpp"

#include "brickengine/json/json.hpp"
#include "exceptions/size_mismatch_exception.hpp"

Scene::Scene(Json json, int screen_width, int screen_height) {
    this->version = json.getDouble("version");
    this->name = json.getString("name");

    calculateRelativeModifier(screen_width, screen_height, json.getInt("width"), json.getInt("height"));

    this->bg_path = json.getString("bg_path");
    this->bg_music = json.getString("bg_music");
}

Scene::Scene(int screen_width, int screen_height, int width, int height) {
    calculateRelativeModifier(screen_width, screen_height, width, height);
}

void Scene::calculateRelativeModifier(int screen_width, int screen_height, int width, int height) {
    double width_ratio = (double)width / (double)screen_width;
    double height_ratio = (double)height / (double)screen_height;

    if(width_ratio != height_ratio) {
        throw SizeMismatchException();
    }
    this->relative_modifier = width_ratio;
}