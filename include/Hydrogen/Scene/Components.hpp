#pragma once

#include <string>
#include "../Core/UUID.hpp"

#include <Vortex/Vortex.hpp>
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
    Vortex::Mesh Mesh = {nullptr};

    MeshRendererComponent() = default;
    MeshRendererComponent(const MeshRendererComponent&) = default;
    MeshRendererComponent(const Vortex::Mesh& mesh) : Mesh(mesh) {
    }
};

struct PointLightComponent {
    Vortex::PointLight PointLight = {1.0f, 1.0f, 1.0f, glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)};

    PointLightComponent() = default;
    // PointLightComponent(const PointLightComponent&) = default;
    PointLightComponent(const Vortex::PointLight& pointLight) : PointLight(pointLight) {
    }
};

struct SpotLightComponent {
    Vortex::SpotLight SpotLight = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)};

    SpotLightComponent() = default;
    SpotLightComponent(const SpotLightComponent&) = default;
    SpotLightComponent(const Vortex::SpotLight& spotLight) : SpotLight(spotLight) {
    }
};

struct DirectionalLightComponent {
    Vortex::DirectionalLight DirectionalLight = {glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)};

    DirectionalLightComponent() = default;
    DirectionalLightComponent(const DirectionalLightComponent&) = default;
    DirectionalLightComponent(const Vortex::DirectionalLight& directionalLight) : DirectionalLight(directionalLight) {
    }
};
} // namespace Hydrogen
