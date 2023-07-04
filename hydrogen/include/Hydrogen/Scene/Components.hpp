#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>

#include "../Core/UUID.hpp"
#include "../Renderer/Renderer.hpp"

namespace Hydrogen {
struct IDComponent {
  UUID ID;

  IDComponent() = default;
  // IDComponent(const IDComponent&) = default;
  IDComponent(const UUID id) : ID(id) {}
};

struct TagComponent {
  String Tag;

  TagComponent() = default;
  // TagComponent(const TagComponent&) = default;
  TagComponent(const String& tag) : Tag(tag) {}
};

struct TransformComponent {
  Vector3 Translation = {0.0f, 0.0f, 0.0f};
  Vector3 Rotation = {0.0f, 0.0f, 0.0f};
  Vector3 Scale = {1.0f, 1.0f, 1.0f};

  TransformComponent() = default;
  // TransformComponent(const TransformComponent&) = default;
  TransformComponent(const Vector3& translation) : Translation(translation) {}

  Matrix4 GetTransform() const {
    Matrix4 rotation = glm::toMat4(glm::quat(Rotation));

    return glm::translate(Matrix4(1.0f), Translation) * rotation *
           glm::scale(Matrix4(1.0f), Scale);
  }
};

struct MeshRendererComponent {
  std::vector<ReferencePointer<VertexArray>> VertexArrays = {};
  ReferencePointer<Shader> MeshShader = {};

  MeshRendererComponent() = default;
  MeshRendererComponent(const MeshRendererComponent&) = default;
};

struct PointLightComponent {
  PointLight _PointLight = {1.0f,          1.0f,          1.0f,
                            Vector3(1.0f), Vector3(1.0f), Vector3(1.0f)};

  PointLightComponent() = default;
  // PointLightComponent(const PointLightComponent&) = default;
  PointLightComponent(const PointLight& pointLight) : _PointLight(pointLight) {}
};

struct SpotLightComponent {
  SpotLight _SpotLight = {1.0f, 1.0f,          1.0f,          1.0f,
                          1.0f, Vector3(1.0f), Vector3(1.0f), Vector3(1.0f)};

  SpotLightComponent() = default;
  SpotLightComponent(const SpotLightComponent&) = default;
  SpotLightComponent(const SpotLight& spotLight) : _SpotLight(spotLight) {}
};

struct DirectionalLightComponent {
  DirectionalLight _DirectionalLight = {Vector3(1.0f), Vector3(1.0f),
                                        Vector3(1.0f)};

  DirectionalLightComponent() = default;
  DirectionalLightComponent(const DirectionalLightComponent&) = default;
  DirectionalLightComponent(const DirectionalLight& directionalLight)
      : _DirectionalLight(directionalLight) {}
};
}  // namespace Hydrogen
