#pragma once

#include "../Renderer/RendererAPI.hpp"
#include "../Core/Memory.hpp"
#include "../Math/Math.hpp"

#include <tracy/tracy.hpp>

namespace Hydrogen {
enum LightType { None = 0, Point = 1, Directional = 2, Spot = 3 };

struct Light {
  Light(const Vector3& ambient, const Vector3& diffuse, const Vector3 specular) : Ambient(ambient), Diffuse(diffuse), Specular(specular) { ZoneScoped; }
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
  Renderer(const ReferencePointer<class RenderWindow>& window, const ReferencePointer<class RenderDevice>& device);
  ~Renderer();

  void Render();

  inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

  static void SetContext(ReferencePointer<class Context> context) { s_Context = context; }
  const ReferencePointer<class Framebuffer>& GetFramebuffer() { return m_Framebuffer; }

  template <typename T>
  static ReferencePointer<T> GetContext() {
    static_assert(std::is_base_of<Context, T>::value, "T must be derived from Context");
    return std::dynamic_pointer_cast<T>(s_Context);
  }

 private:
  static ReferencePointer<Context> s_Context;
  static uint32_t s_MaxFramesInFlight;

  ReferencePointer<class RenderWindow> m_RenderWindow;
  ReferencePointer<class RenderDevice> m_Device;
  ReferencePointer<class SwapChain> m_SwapChain;
  ReferencePointer<class Framebuffer> m_Framebuffer;
  ReferencePointer<class Shader> m_Shader;
  DynamicArray<ReferencePointer<class CommandBuffer>> m_CommandBuffers;
  uint32_t m_CurrentFrame;

  ReferencePointer<class UniformBuffer> m_UniformBuffer;
  ReferencePointer<class VertexBuffer> m_VertexBuffer;
  ReferencePointer<class IndexBuffer> m_IndexBuffer;
  ReferencePointer<class VertexArray> m_VertexArray;
  ReferencePointer<class Texture2D> m_Texture;
};
}  // namespace Hydrogen
