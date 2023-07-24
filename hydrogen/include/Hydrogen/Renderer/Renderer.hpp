#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "Buffer.hpp"
#include "Camera.hpp"
#include "CommandBuffer.hpp"
#include "Context.hpp"
#include "Fence.hpp"
#include "Framebuffer.hpp"
#include "RenderDevice.hpp"
#include "RendererAPI.hpp"
#include "Semaphore.hpp"
#include "Shader.hpp"
#include "SwapChain.hpp"
#include "Texture.hpp"

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
  Renderer(const ReferencePointer<RenderWindow>& window, const ReferencePointer<RenderDevice>& device);
  ~Renderer();

  void Render();

  inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

  static void SetContext(ReferencePointer<Context> context) { s_Context = context; }

  template <typename T>
  static ReferencePointer<T> GetContext() {
    static_assert(std::is_base_of<Context, T>::value, "T must be derived from Context");
    return std::dynamic_pointer_cast<T>(s_Context);
  }
  static ReferencePointer<VertexBuffer> m_VertexBuffer;

 private:
  bool m_FirstFrame;
  static ReferencePointer<Context> s_Context;
  ReferencePointer<SwapChain> m_SwapChain;
  ReferencePointer<RenderPass> m_RenderPass;
  ReferencePointer<Shader> m_Shader;
  ReferencePointer<Framebuffer> m_Framebuffer;
  ReferencePointer<CommandBuffer> m_CommandBuffer;
  ReferencePointer<Semaphore> m_ImageAvailableSemaphore;
  ReferencePointer<Semaphore> m_RenderFinishedSemaphore;
  ReferencePointer<Fence> m_InFlightFence;
};
}  // namespace Hydrogen
