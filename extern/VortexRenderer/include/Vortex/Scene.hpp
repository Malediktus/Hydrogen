#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <tracy/Tracy.hpp>

#include "Camera.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"

namespace Vortex {
struct SceneLight {
    SceneLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3 specular) : Ambient(ambient), Diffuse(diffuse), Specular(specular) {
        ZoneScoped;
    }
    ~SceneLight() = default;

    enum SceneLightType { None = 0, Point = 1, Directional = 2, Spot = 3 };

    SceneLightType Type = SceneLightType::None;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
};

struct PointLight : public SceneLight {
    PointLight(float constant, float linear, float quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3 specular)
        : SceneLight(ambient, diffuse, specular), Constant(constant), Linear(linear), Quadratic(quadratic) {
        ZoneScoped;
        Type = SceneLightType::Point;
    }
    ~PointLight() = default;

    float Constant;
    float Linear;
    float Quadratic;
};

struct DirectionalLight : public SceneLight {
    DirectionalLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3 specular) : SceneLight(ambient, diffuse, specular) {
        ZoneScoped;
        Type = SceneLightType::Directional;
    }
    ~DirectionalLight() = default;
};

struct SpotLight : public SceneLight {
    SpotLight(float cutOff, float outerCutOff, float constant, float linear, float quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3 specular)
        : SceneLight(ambient, diffuse, specular), CutOff(cutOff), OuterCutOff(outerCutOff), Constant(constant), Linear(linear), Quadratic(quadratic) {
        ZoneScoped;
        Type = SceneLightType::Spot;
    }
    ~SpotLight() = default;

    float CutOff;
    float OuterCutOff;
    float Constant;
    float Linear;
    float Quadratic;
};

class Mesh {
public:
    Mesh(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader = nullptr) : m_Shader(shader) {
        ZoneScoped;
        m_VertexArrays.push_back(vertexArray);
    }
    ~Mesh() = default;

    void Append(const std::shared_ptr<VertexArray>& vertexArray) {
        ZoneScoped;
        m_VertexArrays.push_back(vertexArray);
    }

    const std::vector<std::shared_ptr<VertexArray>>& GetVertexArrays() {
        return m_VertexArrays;
    }

    const std::shared_ptr<Shader>& GetShader() {
        return m_Shader;
    }

private:
    std::vector<std::shared_ptr<VertexArray>> m_VertexArrays;
    const std::shared_ptr<Shader>& m_Shader;
};

class Object {
public:
    Object() : m_Transform(1.0f) {
    }
    Object(const glm::mat4& transform) : m_Transform(transform) {
        ZoneScoped;
    }
    ~Object() = default;

    void Attach(const std::shared_ptr<SceneLight>& pointLight) {
        ZoneScoped;
        m_Lights.push_back(pointLight);
    }

    void Attach(const std::shared_ptr<Mesh>& mesh) {
        ZoneScoped;
        m_Meshs.push_back(mesh);
    }

    const glm::mat4& GetTransform() {
        return m_Transform;
    }

    const std::vector<std::shared_ptr<SceneLight>>& GetLights() {
        return m_Lights;
    }

    const std::vector<std::shared_ptr<Mesh>>& GetMeshs() {
        return m_Meshs;
    }

private:
    glm::mat4 m_Transform;
    std::vector<std::shared_ptr<SceneLight>> m_Lights;
    std::vector<std::shared_ptr<Mesh>> m_Meshs;
};

class Scene {
public:
    Scene() {
    }
    ~Scene() = default;

    void Append(const std::shared_ptr<Object>& object) {
        ZoneScoped;
        m_Objects.push_back(object);
    }

    const std::vector<std::shared_ptr<Object>>& GetObjects() {
        return m_Objects;
    }

private:
    std::vector<std::shared_ptr<Object>> m_Objects;
};
} // namespace Vortex
