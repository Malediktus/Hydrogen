#include <Hydrogen/Scene/Scene.hpp>

using namespace Hydrogen;

ReferencePointer<Entity> Scene::AddEntity(const String& name) {
    auto entity = NewReferencePointer<Entity>(this);
    entity->AddComponent<TagComponent>(name);
    entity->AddComponent<IDComponent>(UUID());
    return entity;
}
