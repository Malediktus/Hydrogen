#pragma once

#include <vector>

#include "RendererAPI.hpp"

namespace Hydrogen {
class RenderWindow {
 public:
  virtual ~RenderWindow() = default;

  virtual Vector2 GetViewportSize() const = 0;
  virtual void SetupImGui() = 0;
  virtual void ImGuiNewFrame() = 0;
  virtual void DestroyImGui() = 0;
  virtual void UpdateImGuiPlatformWindows() = 0;
  virtual void SetupOpenglContext(int majorVersion, int minorVersion) = 0;
  virtual void* GetWindowOpenGLProcAddress() = 0;
  virtual const std::vector<const char*> GetVulkanWindowExtensions() = 0;
  virtual void* GetVulkanWindowSurface() = 0;

  static RendererAPI::API ChooseRenderingAPI(bool vulkanAvailable);
};
}  // namespace Hydrogen
