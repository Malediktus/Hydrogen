#pragma once

#include <entt/entt.hpp>

#include "../Core/Memory.hpp"

namespace Hydrogen {
class Entity;

class Scene {
 public:
  Scene(const String& name);
  ~Scene();

  entt::registry* GetRegistry() { return &m_Registry; }

  Entity CreateEntity(const String& name);
  Entity CreateEntity(const String& name, const String& tag);
  void DestroyEntity(Entity entity);

  DynamicArray<Entity> GetEntities();
  DynamicArray<Entity> GetEntitiesByName(const String& name);
  DynamicArray<Entity> GetEntitiesByTag(const String& tag);

  const String& GetName() const { return m_Name; }

 private:
  String m_Name;
  entt::registry m_Registry;
};
}  // namespace Hydrogen
