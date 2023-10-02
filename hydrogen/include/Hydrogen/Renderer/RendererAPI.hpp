#pragma once

#include <imgui.h>

#include "../Core/Memory.hpp"

namespace Hydrogen {
class RendererAPI {
 public:
  enum class API { OpenGL = 0, Vulkan = 1 };

  virtual void SetupImGui() = 0;
  virtual void ImGuiNewFrame() = 0;
  virtual void DestroyImGui() = 0;

  inline static RendererAPI::API GetAPI() { return s_API; }
  inline static void SetAPI(RendererAPI::API api) { s_API = api; }

  static ReferencePointer<RendererAPI> Create(const ReferencePointer<class RenderDevice>& renderDevice, const ReferencePointer<class Framebuffer>& framebuffer);

 private:
  static API s_API;
};
}  // namespace Hydrogen
