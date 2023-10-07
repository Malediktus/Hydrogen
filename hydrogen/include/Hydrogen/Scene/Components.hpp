#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "../Core/Memory.hpp"

namespace Hydrogen {
class Entity;

struct TagComponent {
  TagComponent() = default;
  ~TagComponent() = default;

  TagComponent(TagComponent& other) : Name(other.Name), Tag(other.Tag), UUID(0) {}

  TagComponent(const String& name) : Name(name), Tag(""), UUID(0) {}
  TagComponent(const String& name, const String& tag) : Name(name), Tag(tag), UUID(0) {}

  String Name;
  String Tag;
  uint64_t UUID;
};

struct TransformComponent {
  TransformComponent() = default;
  ~TransformComponent() = default;

  TransformComponent(TransformComponent&) = default;

  TransformComponent(const glm::vec3& translation) : Translation(translation) {}

  glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
  glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
  glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

  glm::mat4 GetTransform() const {
    glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

    return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
  }
};

struct HierarchyComponent {
  HierarchyComponent() = default;
  ~HierarchyComponent() = default;

  HierarchyComponent(HierarchyComponent& other) : Parent(other.Parent), Children(other.Children) {}

  HierarchyComponent(Entity parent) : Parent(parent) {}

  void AddChild(Entity child) { Children.push_back(child); }

  class Entity Parent;
  DynamicArray<class Entity> Children;
};

struct MeshRendererComponent {
  MeshRendererComponent() = default;
  ~MeshRendererComponent() = default;

  MeshRendererComponent(MeshRendererComponent&) = default;

  DynamicArray<ReferencePointer<class VertexArray>> VertexArrays;
};
}  // namespace Hydrogen
