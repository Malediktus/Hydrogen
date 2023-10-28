#include <Hydrogen/Scene/Components.hpp>
#include <Hydrogen/Scene/Entity.hpp>
#include <Hydrogen/Scene/Scene.hpp>

using namespace Hydrogen;

Entity::Entity(Scene* scene, const entt::entity entity) : m_EntityHandle(entity), m_Scene(scene) {}

Entity::Entity() : m_EntityHandle(entt::null), m_Scene(nullptr) {}

DynamicArray<Entity> Entity::GetChildren() { return GetComponent<HierarchyComponent>().Children; }

DynamicArray<Entity> Entity::GetChildrenByName(const String& name) {
  DynamicArray<Entity> entities;
  for (auto& child : GetComponent<HierarchyComponent>().Children) {
    if (child.GetComponent<TagComponent>().Name == name) {
      entities.push_back(child);
    }
  }

  return entities;
}

DynamicArray<Entity> Entity::GetChildrenByTag(const String& tag) {
  DynamicArray<Entity> entities;
  for (auto& child : GetComponent<HierarchyComponent>().Children) {
    if (child.GetComponent<TagComponent>().Tag == tag) {
      entities.push_back(child);
    }
  }

  return entities;
}

Entity Entity::CreateChild(const String& name) {
  auto entity = m_Scene->CreateEntity(name);

  GetComponent<HierarchyComponent>().AddChild(entity);
  entity.GetComponent<HierarchyComponent>().Parent = *this;

  return entity;
}

Entity Entity::CreateChild(const String& name, const String& tag) {
  auto entity = m_Scene->CreateEntity(name, tag);

  GetComponent<HierarchyComponent>().AddChild(entity);
  entity.GetComponent<HierarchyComponent>().Parent = *this;

  return entity;
}
