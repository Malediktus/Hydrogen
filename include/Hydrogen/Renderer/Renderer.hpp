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
    Light(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3 specular) : Ambient(ambient), Diffuse(diffuse), Specular(specular) {
        ZoneScoped;
    }
    ~Light() = default;

    LightType Type = LightType::None;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
};

struct PointLight : public Light {
    PointLight(float constant, float linear, float quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3 specular)
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
    DirectionalLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3 specular) : Light(ambient, diffuse, specular) {
        ZoneScoped;
        Type = LightType::Directional;
    }
    ~DirectionalLight() = default;
};

struct SpotLight : public Light {
    SpotLight(float cutOff, float outerCutOff, float constant, float linear, float quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3 specular)
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
    Renderer(const Reference<Shader>& defaultShader, const int width, const int height, bool renderToTexture);
    ~Renderer() = default;

    void OnResize(const int width, const int height);

    void BeginFrame(const Reference<Camera>& camera);
    void EndFrame();

    void Submit(const Reference<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
    void Submit(const Light& light, const glm::mat4& transform = glm::mat4(1.0f));

    Reference<Texture2D> GetTexture() {
        return m_ColorTexture;
    }

    inline static RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }

    static void SetContext(Reference<Context> context);
    static Reference<Context> GetContext();

private:
    bool m_RenderToTexture;
    Reference<Shader> m_Shader;
    Reference<Framebuffer> m_Framebuffer;
    Reference<Texture2D> m_ColorTexture;
    Reference<Renderbuffer> m_DepthStencilRenderbuffer;
    static Reference<Context> s_Context;

    struct Framedata {
        Reference<Camera> FrameCamera;
        std::vector<Reference<VertexArray>> VertexArrays;
        std::vector<glm::mat4> VertexArrayTransforms;
        std::vector<Light> Lights;
        std::vector<glm::mat4> LightTransforms;
    } m_Framedata;
};
} // namespace Hydrogen
