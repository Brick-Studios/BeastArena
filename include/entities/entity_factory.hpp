#ifndef FILE_ENTITY_FACTORY_HPP
#define FILE_ENTITY_FACTORY_HPP

#include <vector>
#include <string>
#include <utility>

#include "menu/button.hpp"
#include "brickengine/entities/entity_manager.hpp"
#include "brickengine/rendering/renderables/texture.hpp"
#include "brickengine/rendering/renderable_factory.hpp"
#include "entities/layers.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"

class EntityFactory {
public:
    EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf);
    ~EntityFactory() = default;
    int createGorilla(double x_pos, double y_pos, int player_id) const;
    int createPanda1(double x_pos, double y_pos, int player_id) const;
    int createPanda2(double x_pos, double y_pos, int player_id) const;
    int createPanda3(double x_pos, double y_pos, int player_id) const;
    int createWeapon(double x_pos, double y_pos, bool ammo) const;
    int createImage(std::string path, int x_pos, int y_pos, int x_scale, int y_scale, Layers layer, int alpha);
    int createPlatform(double x_pos, double y_pos, double x_scale, double y_scale, std::string path, int alpha);
    std::pair<int, int> createButton(const Button button, const double relative_modifier);
private:
    std::shared_ptr<EntityManager> entityManager;
    RenderableFactory& renderableFactory;
    inline static const std::string graphicsPath = "./assets/graphics/";
};

#endif // FILE_ENTITY_FACTORY_HPP
