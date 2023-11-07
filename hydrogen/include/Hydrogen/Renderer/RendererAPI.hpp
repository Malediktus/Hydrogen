#pragma once

#include <imgui.h>

#include "../Core/Memory.hpp"

namespace Hydrogen {
class RendererAPI {
 public:
  enum class API { OpenGL = 0, Vulkan = 1 };

  static const ReferencePointer<RendererAPI>& Get() { return s_Instance; }
  static void ReleaseInstance() { s_Instance = nullptr; }

  virtual void SetupImGui() = 0;
  virtual void ImGuiNewFrame() = 0;
  virtual void DestroyImGui() = 0;

  virtual void BeginFrame() = 0;
  virtual void EndFrame() = 0;
  virtual void DrawIndexed(const ReferencePointer<class VertexArray>& vertexArray) = 0;
  virtual void SetViewport(uint32_t width = UINT32_MAX, uint32_t height = UINT32_MAX) = 0;
  virtual void SetScissor(int offsetX = 0, int offsetY = 0) = 0;

  inline static RendererAPI::API GetAPI() { return s_API; }
  inline static void SetAPI(RendererAPI::API api) { s_API = api; }

  static ReferencePointer<RendererAPI> Create(const ReferencePointer<class RenderWindow>& window);

 private:
  static ReferencePointer<RendererAPI> s_Instance;
  static API s_API;
};
}  // namespace Hydrogen
