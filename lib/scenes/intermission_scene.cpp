#include "scenes/intermission_scene.hpp"
#include "brickengine/components/renderables/texture_component.hpp"

IntermissionScene::IntermissionScene(int timer, EntityFactory& entity_factory)
    : entity_factory(entity_factory), timer(timer) { }

void IntermissionScene::performPrepare(){
    entity_components = std::make_unique<std::vector<std::unique_ptr<std::vector<std::unique_ptr<Component>>>>>();
    {
        auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
        auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
        auto r_text = entity_factory.getRenderableFactory().createText(std::to_string(timer), 50, { 0, 255, 0, 255 }, (int)Layers::UI, std::move(dst));
        comps->push_back(std::make_unique<TransformComponent>(800, 450, 300, 300, Direction::POSITIVE, Direction::POSITIVE));
        comps->push_back(std::make_unique<TextureComponent>(std::move(r_text)));
        entity_components->push_back(std::move(comps));
    }
}
void IntermissionScene::start() {
    // Nothing
}
void IntermissionScene::leave() {
    // Nothing
}
