#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Core/UUID.hpp>
#include <Hydrogen/Scene/Components.hpp>
#include <Hydrogen/Scene/Entity.hpp>
#include <Hydrogen/Scene/Scene.hpp>

using namespace Hydrogen;

Scene::Scene(const std::string& name) : m_Name(name), m_Registry() {}

Scene::~Scene() {}

Entity Scene::CreateEntity(const std::string& name) {
  auto entityHandle = m_Registry.create();

  Entity entity(this, entityHandle);
  entity.AddComponent<TagComponent>(name).UUID = UUID().GetValue();
  entity.AddComponent<TransformComponent>();
  entity.AddComponent<HierarchyComponent>(Entity());

  return entity;
}

Entity Scene::CreateEntity(const std::string& name, const std::string& tag) {
  auto entityHandle = m_Registry.create();

  Entity entity(this, entityHandle);
  entity.AddComponent<TagComponent>(name, tag).UUID = UUID().GetValue();
  entity.AddComponent<TransformComponent>();
  entity.AddComponent<HierarchyComponent>(Entity());

  return entity;
}

void Scene::DestroyEntity(Entity entity) {
  HY_ASSERT(entity.GetScene() == this, "Entity is not present in this scene!");
  m_Registry.destroy(entity.GetEntityHandle());
}

DynamicArray<Entity> Scene::GetEntities() {
  auto view = m_Registry.view<TagComponent>();

  DynamicArray<Entity> entities;
  for (auto& entity : view) {
    entities.push_back({this, entity});
  }

  return entities;
}

DynamicArray<Entity> Scene::GetEntitiesByName(const std::string& name) {
  auto view = m_Registry.view<TagComponent>();

  DynamicArray<Entity> entities;
  for (auto& entity : view) {
    if (m_Registry.get<TagComponent>(entity).Name == name) {
      entities.push_back({this, entity});
    }
  }

  return entities;
}

DynamicArray<Entity> Scene::GetEntitiesByTag(const std::string& tag) {
  auto view = m_Registry.view<TagComponent>();

  DynamicArray<Entity> entities;
  for (auto& entity : view) {
    if (m_Registry.get<TagComponent>(entity).Tag == tag) {
      entities.push_back({this, entity});
    }
  }

  return entities;
}
