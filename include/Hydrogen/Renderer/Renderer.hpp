#pragma once

#include "RenderCommand.hpp"
#include "RendererAPI.hpp"
#include "Shader.hpp"
#include "Framebuffer.hpp"
#include "Texture.hpp"
#include "Context.hpp"
#include "Camera.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace Hydrogen {
enum LightType { None = 0, Point = 1, Directional = 2, Spot = 3 };

struct Light {
    Light(const Vector3& ambient, const Vector3& diffuse, const Vector3 specular) : Ambient(ambient), Diffuse(diffuse), Specular(specular) {
        ZoneScoped;
    }
    ~Light() = default;

    LightType Type = LightType::None;
    Vector3 Ambient;
    Vector3 Diffuse;
    Vector3 Specular;
};

struct PointLight : public Light {
    PointLight(float constant, float linear, float quadratic, const Vector3& ambient, const Vector3& diffuse, const Vector3 specular)
        : Light(ambient, diffuse, specular), Constant(constant), Linear(linear), Quadratic(quadratic) {
        ZoneScoped;
        Type = LightType::Point;
    }
    ~PointLight() = default;

    float Constant;
    float Linear;
    float Quadratic;
};

struct DirectionalLight : public Light {
    DirectionalLight(const Vector3& ambient, const Vector3& diffuse, const Vector3 specular) : Light(ambient, diffuse, specular) {
        ZoneScoped;
        Type = LightType::Directional;
    }
    ~DirectionalLight() = default;
};

struct SpotLight : public Light {
    SpotLight(float cutOff, float outerCutOff, float constant, float linear, float quadratic, const Vector3& ambient, const Vector3& diffuse, const Vector3 specular)
        : Light(ambient, diffuse, specular), CutOff(cutOff), OuterCutOff(outerCutOff), Constant(constant), Linear(linear), Quadratic(quadratic) {
        ZoneScoped;
        Type = LightType::Spot;
    }
    ~SpotLight() = default;

    float CutOff;
    float OuterCutOff;
    float Constant;
    float Linear;
    float Quadratic;
};

class Renderer {
public:
    Renderer();
    ~Renderer() = default;

    inline static RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }

    static void SetContext(ReferencePointer<Context> context);
    static ReferencePointer<Context> GetContext();

private:
    static ReferencePointer<Context> s_Context;
};
} // namespace Hydrogen
