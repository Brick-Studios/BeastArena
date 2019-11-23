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
    int createPanda(double x_pos, double y_pos, int player_id) const;
    int createCheetah(double x_pos, double y_pos, int player_id) const;
    int createElephant(double x_pos, double y_pos, int player_id) const;
    int createPistol(double x_pos, double y_pos, bool ammo) const;
    int createRifle(double x_pos, double y_pos, bool ammo) const;
    int createSniper(double x_pos, double y_pos, bool ammo) const;
    int createCritter(double x_pos, double y_pos) const;
    int createImage(std::string path, int x_pos, int y_pos, int x_scale, int y_scale, Layers layer, int alpha);
    int createPlatform(double x_pos, double y_pos, double x_scale, double y_scale, std::string path, int alpha);
    std::pair<int, int> createButton(const Button button, const double relative_modifier);
    int createText(std::string text, int x, int y, int x_scale, int y_scale);
private:
    std::shared_ptr<EntityManager> entityManager;
    RenderableFactory& renderableFactory;
    inline static const std::string graphicsPath = "./assets/graphics/";
};

#endif // FILE_ENTITY_FACTORY_HPP
