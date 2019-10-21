#ifndef FILE_ENTITY_FACTORY_HPP
#define FILE_ENTITY_FACTORY_HPP

#include <vector>

#include "brickengine/entities/entity_manager.hpp"
#include "brickengine/rendering/renderables/texture.hpp"
#include "brickengine/rendering/renderable_factory.hpp"
#include "entities/layers.hpp"

class EntityFactory {
public:
    EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf);
    ~EntityFactory() = default;
    int createPanda(double x, double y, int playerId) const;
    int createGorilla(double x, double y, int playerId) const;
    int createImage(std::string path, int x, int y, int width, int height, Layers layer);
    int createPlatform(double xScale, double yScale, double x, double y);
private:
    std::shared_ptr<EntityManager> entityManager;
    RenderableFactory& renderableFactory;
    inline static const std::string graphicsPath = "./assets/graphics/";
};

#endif // FILE_ENTITY_FACTORY_HPP
