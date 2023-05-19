#include <Hydrogen/Scene/Scene.hpp>

using namespace Hydrogen;

Reference<Entity> Scene::AddEntity(const std::string& name) {
    auto entity = NewReference<Entity>(this);
    entity->AddComponent<TagComponent>(name);
    entity->AddComponent<IDComponent>(UUID());
    return entity;
}
