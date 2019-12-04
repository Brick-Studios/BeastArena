#ifndef FILE_ENTITY_FACTORY_HPP
#define FILE_ENTITY_FACTORY_HPP

#include <vector>
#include <string>
#include <utility>

#include "brickengine/entities/entity_manager.hpp"
#include "brickengine/rendering/renderables/texture.hpp"
#include "brickengine/rendering/renderable_factory.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "entities/layers.hpp"
#include "components/health_component.hpp"
#include "components/spawn_component.hpp"
#include "scenes/data/menu/button.hpp"
#include "enums/character.hpp"
#include "entities/character_specs.hpp"
#include "enums/gadget_type.hpp"

class EntityFactory {
public:
    EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf);
    ~EntityFactory() = default;

    EntityManager& getEntityManager() {
        return *entityManager;
    }
    RenderableFactory& getRenderableFactory() {
        return renderableFactory;
    };

    int createPlayer(int player_id, Character character, int x = -2000, int y = -2000) const;
    int createGorilla(int player_id) const;
    int createPanda(int player_id) const;
    int createCheetah(int player_id) const;
    int createElephant(int player_id) const;
    int createCritter(double x_pos, double y_pos) const;
    int createSpawner(double x_pos, double y_pos, std::vector<GadgetType> available_spawns, int respawn_timer,
                      bool always_respawn)  const;
    int createImage(std::string path, int x_pos, int y_pos, int x_scale, int y_scale, Layers layer, int alpha);
    int createPlatform(double x_pos, double y_pos, double x_scale, double y_scale, std::string path, int alpha);
    std::pair<int, int> createButton(const Button button, const double relative_modifier);
    int createText(std::string text, int x, int y, int x_scale, int y_scale, int font_size, Color color);
    int createCharacterSelector(int player_id, int x, int y);
    void changeCharacterSelectorTexture(int entity_id, Character character, bool create);

    const std::vector<Character> getAvailableCharacters() const;
    const std::vector<std::pair<Character, bool>> getPickedCharacters() const;
private:
    std::shared_ptr<EntityManager> entityManager;
    RenderableFactory& renderableFactory;

    inline static const std::string GRAPHICS_PATH = "./assets/graphics/";
    inline static const int POINTS_ON_KILL_PLAYER = 10;
    HealthComponent::EntityFunction player_on_death;
    HealthComponent::EntityFunction player_revive;

    const CharacterSpecs getCharacterSpecs(Character character) const;
    SpawnComponent::CreateCompsFn createPistolComponents;
    SpawnComponent::CreateCompsFn createRifleComponents;
    SpawnComponent::CreateCompsFn createSniperComponents;
};

#endif // FILE_ENTITY_FACTORY_HPP