#pragma once

#include <tracy/tracy.hpp>

#include "../Core/Memory.hpp"
#include "../Math/Math.hpp"
#include "../Renderer/RendererAPI.hpp"

namespace Hydrogen {
class Renderer {
 public:
  Renderer(const ReferencePointer<class RenderWindow>& window, const ScopePointer<class Scene>& scene);
  ~Renderer();

  void Render();
  void RenderMesh(const ReferencePointer<class CommandBuffer>& commandBuffer, const ReferencePointer<class VertexArray>& vertexArray);

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
  static uint32_t s_MaxFramesInFlight;

  const ScopePointer<class Scene>& m_Scene;
  ReferencePointer<class RenderWindow> m_RenderWindow;
  ReferencePointer<class SwapChain> m_SwapChain;
  ReferencePointer<class Framebuffer> m_Framebuffer;
  ReferencePointer<class Shader> m_Shader;
  DynamicArray<ReferencePointer<class CommandBuffer>> m_CommandBuffers;
  uint32_t m_CurrentFrame;

  ReferencePointer<class UniformBuffer> m_UniformBuffer;
  ReferencePointer<class UniformBuffer> m_LightBuffer;
  ReferencePointer<class Texture2D> m_WhiteTexture;
};
}  // namespace Hydrogen
