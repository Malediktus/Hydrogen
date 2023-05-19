#pragma once

#include "../Core/UUID.hpp"
#include "../Renderer/Renderer.hpp"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Hydrogen {
struct IDComponent {
    UUID ID;

    IDComponent() = default;
    // IDComponent(const IDComponent&) = default;
    IDComponent(const UUID id) : ID(id) {
    }
};

struct TagComponent {
    std::string Tag;

    TagComponent() = default;
    // TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag) : Tag(tag) {
    }
};

struct TransformComponent {
    glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

    TransformComponent() = default;
    // TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& translation) : Translation(translation) {
    }

    glm::mat4 GetTransform() const {
        glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

        return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
    }
};

struct MeshRendererComponent {
    std::vector<Reference<VertexArray>> VertexArrays = {};
    Reference<Shader> MeshShader = {};

    MeshRendererComponent() = default;
    MeshRendererComponent(const MeshRendererComponent&) = default;
};

struct PointLightComponent {
    PointLight _PointLight = {1.0f, 1.0f, 1.0f, glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)};

    PointLightComponent() = default;
    // PointLightComponent(const PointLightComponent&) = default;
    PointLightComponent(const PointLight& pointLight) : _PointLight(pointLight) {
    }
};

struct SpotLightComponent {
    SpotLight _SpotLight = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)};

    SpotLightComponent() = default;
    SpotLightComponent(const SpotLightComponent&) = default;
    SpotLightComponent(const SpotLight& spotLight) : _SpotLight(spotLight) {
    }
};

struct DirectionalLightComponent {
    DirectionalLight _DirectionalLight = {glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)};

    DirectionalLightComponent() = default;
    DirectionalLightComponent(const DirectionalLightComponent&) = default;
    DirectionalLightComponent(const DirectionalLight& directionalLight) : _DirectionalLight(directionalLight) {
    }
};
} // namespace Hydrogen
