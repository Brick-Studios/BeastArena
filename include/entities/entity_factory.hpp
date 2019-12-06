#ifndef FILE_ENTITY_FACTORY_HPP
#define FILE_ENTITY_FACTORY_HPP

#include <vector>
#include <string>
#include <utility>
#include <optional>

#include "brickengine/entities/entity_manager.hpp"
#include "brickengine/rendering/renderables/texture.hpp"
#include "brickengine/rendering/renderable_factory.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
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

    EntityComponents createPlayer(int player_id, Character character, int x = -2000, int y = -2000) const;
    EntityComponents createCritter(double x_pos, double y_pos) const;
    EntityComponents createImage(std::string path, int x_pos, int y_pos, int x_scale, int y_scale, double relative_modifier, Layers layer, int alpha);
    EntityComponents createPlatform(double x_pos, double y_pos, double x_scale, double y_scale, double relative_modifier, std::string path, int alpha);
    EntityComponents createTrophy(int x, int y, int x_scale, int y_scale, double relative_modifier, Layers layer, int alpha);
    EntityComponents createReadySign(int x, int y, int x_scale, int y_scale, double relative_modifier, Layers layer, int alpha);
    std::vector<EntityComponents> createButton(std::string text, Color text_color, int font_size,
        std::string texture_path, int x, int y, int x_scale, int y_scale, 
        int alpha, double relative_modifier, std::function<void ()> on_click);
    EntityComponents createText(std::string text, Color color, int font_size, int x, int y, int x_scale, int y_scale, double relative_modifier);

    // STOP! This function is only meant to be used for systems and the start function within scenes. Use the entity_components list whenever possible.
    int addToEntityManager(EntityComponents entity_components, std::optional<std::pair<int,bool>> parent_opt = std::nullopt, std::optional<std::string> scene_tag = std::nullopt);

    EntityComponents createSpawner(double x_pos, double y_pos, double relative_modifier, std::vector<GadgetType> available_spawns, int respawn_timer,
                      bool always_respawn)  const;
    EntityComponents createCharacterSelector(int player_id, int x, int y, double relative_modifier);
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