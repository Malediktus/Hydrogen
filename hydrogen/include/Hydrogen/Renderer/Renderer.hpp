#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "Camera.hpp"
#include "Context.hpp"
#include "Framebuffer.hpp"
#include "RenderCommand.hpp"
#include "RenderDevice.hpp"
#include "RendererAPI.hpp"
#include "Shader.hpp"
#include "SwapChain.hpp"
#include "Texture.hpp"

namespace Hydrogen {
enum LightType { None = 0, Point = 1, Directional = 2, Spot = 3 };

struct Light {
  Light(const Vector3& ambient, const Vector3& diffuse, const Vector3 specular)
      : Ambient(ambient), Diffuse(diffuse), Specular(specular) {
    ZoneScoped;
  }
  ~Light() = default;

  LightType Type = LightType::None;
  Vector3 Ambient;
  Vector3 Diffuse;
  Vector3 Specular;
};

struct PointLight : public Light {
  PointLight(float constant, float linear, float quadratic,
             const Vector3& ambient, const Vector3& diffuse,
             const Vector3 specular)
      : Light(ambient, diffuse, specular),
        Constant(constant),
        Linear(linear),
        Quadratic(quadratic) {
    ZoneScoped;
    Type = LightType::Point;
  }
  ~PointLight() = default;

  float Constant;
  float Linear;
  float Quadratic;
};

struct DirectionalLight : public Light {
  DirectionalLight(const Vector3& ambient, const Vector3& diffuse,
                   const Vector3 specular)
      : Light(ambient, diffuse, specular) {
    ZoneScoped;
    Type = LightType::Directional;
  }
  ~DirectionalLight() = default;
};

struct SpotLight : public Light {
  SpotLight(float cutOff, float outerCutOff, float constant, float linear,
            float quadratic, const Vector3& ambient, const Vector3& diffuse,
            const Vector3 specular)
      : Light(ambient, diffuse, specular),
        CutOff(cutOff),
        OuterCutOff(outerCutOff),
        Constant(constant),
        Linear(linear),
        Quadratic(quadratic) {
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

  inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

  static void SetContext(ReferencePointer<Context> context) {
    s_Context = context;
  }

  static void SetRenderDevice(ReferencePointer<RenderDevice> renderDevice) {
    s_RenderDevice = renderDevice;
  }

  static void SetSwapChain(ReferencePointer<SwapChain> swapChain) {
    s_SwapChain = swapChain;
  }

  template <typename T>
  static ReferencePointer<T> GetContext() {
    static_assert(std::is_base_of<Context, T>::value,
                  "T must be derived from Context");
    return std::dynamic_pointer_cast<T>(s_Context);
  }

  template <typename T>
  static ReferencePointer<T> GetRenderDevice() {
    static_assert(std::is_base_of<RenderDevice, T>::value,
                  "T must be derived from RenderDevice");
    return std::dynamic_pointer_cast<T>(s_RenderDevice);
  }

  template <typename T>
  static ReferencePointer<T> GetSwapChain() {
    static_assert(std::is_base_of<SwapChain, T>::value,
                  "T must be derived from SwapChain");
    return std::dynamic_pointer_cast<T>(s_SwapChain);
  }

 private:
  static ReferencePointer<Context> s_Context;
  static ReferencePointer<RenderDevice> s_RenderDevice;
  static ReferencePointer<SwapChain> s_SwapChain;
};
}  // namespace Hydrogen
