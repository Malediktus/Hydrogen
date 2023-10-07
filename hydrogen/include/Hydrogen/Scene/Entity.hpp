#pragma once

#include "../Core/Assert.hpp"
#include "Scene.hpp"
#include <entt/entt.hpp>

namespace Hydrogen {
class Entity {
 public:
  Entity(Scene* scene, const entt::entity entity);
  Entity();
  ~Entity() = default;

  entt::entity GetEntityHandle() { return m_EntityHandle; }
  Scene* GetScene() { return m_Scene; }

  DynamicArray<Entity> GetChildren();
  DynamicArray<Entity> GetChildrenByName(const String& name);
  DynamicArray<Entity> GetChildrenByTag(const String& tag);

  Entity CreateChild(const String& name);
  Entity CreateChild(const String& name, const String& tag);

  bool Exists() { return m_EntityHandle != entt::null; }

  template <typename T>
  bool HasComponent() {
    return m_Scene->GetRegistry()->try_get<T>(m_EntityHandle);
  }

  template <typename T, typename... Args>
  T& AddComponent(Args&&... args) {
    HY_ASSERT(!HasComponent<T>(), "Entity already has component!");
    return m_Scene->GetRegistry()->emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  T& AddOrReplaceComponent(Args&&... args) {
    return m_Scene->GetRegistry()->emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
  }

  template <typename T>
  T& GetComponent() {
    HY_ASSERT(HasComponent<T>(), "Entity does not have given component");
    return m_Scene->GetRegistry()->get<T>(m_EntityHandle);
  }

  template <typename T>
  T& GetComponentOrNull() {
    return m_Scene->GetRegistry()->get<T>(m_EntityHandle);
  }

  template <typename T>
  void RemoveComponent() {
    HY_ASSERT(HasComponent<T>(), "Entity does not have component!");
    m_Scene->GetRegistry()->remove<T>(m_EntityHandle);
  }

 private:
  entt::entity m_EntityHandle;
  Scene* m_Scene;
};
}  // namespace Hydrogen
