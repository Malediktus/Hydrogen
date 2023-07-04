#pragma once

#include <entt/entt.hpp>

#include "../Core/Assert.hpp"
#include "../Core/Memory.hpp"
#include "../Core/UUID.hpp"
#include "Components.hpp"

namespace Hydrogen {
class Entity;

class Scene {
 public:
  Scene() = default;
  ~Scene() = default;

  ReferencePointer<Entity> AddEntity(const String& name);

 private:
  entt::registry m_Registry;

  friend class Entity;
};

class Entity {
 public:
  Entity() = default;
  ~Entity() { m_Scene->m_Registry.destroy(m_EntityHandle); }
  Entity(const Entity& other) = default;
  Entity(Scene* scene) : m_Scene(scene) {
    m_EntityHandle = m_Scene->m_Registry.create();
  }

  template <typename T>
  bool HasComponent() {
    return m_Scene->m_Registry.try_get<T>(m_EntityHandle);
  }

  template <typename T, typename... Args>
  T& AddComponent(Args&&... args) {
    HY_ASSERT(!HasComponent<T>(), "Entity already has component!");
    return m_Scene->m_Registry.emplace<T>(m_EntityHandle,
                                          std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  T& AddOrReplaceComponent(Args&&... args) {
    return m_Scene->m_Registry.emplace_or_replace<T>(
        m_EntityHandle, std::forward<Args>(args)...);
  }

  template <typename T>
  T& GetComponent() {
    HY_ASSERT(HasComponent<T>(), "Entity does not have given component");
    return m_Scene->m_Registry.get<T>(m_EntityHandle);
  }

  template <typename T>
  T& GetComponentOrNull() {
    return m_Scene->m_Registry.get<T>(m_EntityHandle);
  }

  template <typename T>
  void RemoveComponent() {
    HY_ASSERT(HasComponent<T>(), "Entity does not have component!");
    m_Scene->m_Registry.remove<T>(m_EntityHandle);
  }

  operator bool() const { return m_EntityHandle != entt::null; }

  UUID GetUUID() { return GetComponent<IDComponent>().ID; }

  const String& GetName() { return GetComponent<TagComponent>().Tag; }

  bool operator==(const Entity& other) const {
    return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
  }

  bool operator!=(const Entity& other) const { return !(*this == other); }

 private:
  Scene* m_Scene;
  entt::entity m_EntityHandle;

  friend class Scene;
};
}  // namespace Hydrogen
