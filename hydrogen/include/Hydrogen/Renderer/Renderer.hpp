#pragma once

#include <tracy/tracy.hpp>

#include "../Core/Memory.hpp"
#include "../Math/Math.hpp"
#include "../Renderer/RendererAPI.hpp"

namespace Hydrogen {
class Renderer {
 public:
  Renderer(const ReferencePointer<class RenderWindow>& window);
  ~Renderer();

  void BeginFrame();
  void Submit(const ReferencePointer<class VertexArray>& vertexArray, const ReferencePointer<class Shader>& shader, const glm::mat4& transform);
  void EndFrame();

  inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

  static void SetContext(const ReferencePointer<class Context>& context) { s_Context = context; }

  template <typename T>
  static ReferencePointer<T> GetContext() {
    static_assert(std::is_base_of<Context, T>::value, "T must be derived from Context");
    return std::dynamic_pointer_cast<T>(s_Context);
  }

  static void SetRenderDevice(const ReferencePointer<class RenderDevice>& renderDevice) { s_RenderDevice = renderDevice; }

  template <typename T>
  static ReferencePointer<T> GetRenderDevice() {
    static_assert(std::is_base_of<RenderDevice, T>::value, "T must be derived from RenderDevice");
    return std::dynamic_pointer_cast<T>(s_RenderDevice);
  }

 private:
  static ReferencePointer<Context> s_Context;
  static ReferencePointer<RenderDevice> s_RenderDevice;

  ReferencePointer<class RenderWindow> m_RenderWindow;

  // TODO: Remove
  ReferencePointer<class Shader> m_Shader;
  ReferencePointer<class UniformBuffer> m_UniformBuffer;
  ReferencePointer<class UniformBuffer> m_LightBuffer;
  ReferencePointer<class UniformBuffer> m_ObjectInformation;
  ReferencePointer<class Texture2D> m_WhiteTexture;
};
}  // namespace Hydrogen
