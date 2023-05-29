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
    Renderer(const ReferencePointer<Shader>& defaultShader, const int width, const int height);
    ~Renderer() = default;

    void OnResize(const int width, const int height);

    void BeginFrame(const ReferencePointer<Camera>& camera);
    void EndFrame();

    void Submit(const ReferencePointer<VertexArray>& vertexArray, const Matrix4& transform = Matrix4(1.0f));
    void Submit(const Light& light, const Matrix4& transform = Matrix4(1.0f));

    inline static RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }

    static void SetContext(ReferencePointer<Context> context);
    static ReferencePointer<Context> GetContext();

private:
    ReferencePointer<Shader> m_Shader;
    static ReferencePointer<Context> s_Context;

    struct Framedata {
        ReferencePointer<Camera> FrameCamera;
        DynamicArray<const ReferencePointer<VertexArray>> vertexArrays;
        DynamicArray<Matrix4> vertexArrayTransforms;
        DynamicArray<Light> lights;
        DynamicArray<Matrix4> lightTransforms;
    } m_Framedata;
};
} // namespace Hydrogen
