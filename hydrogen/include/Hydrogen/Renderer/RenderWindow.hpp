#pragma once

#include <vector>

#include "../Math/Math.hpp"
#include "RendererAPI.hpp"
#include "SurfaceAttachment.hpp"

namespace Hydrogen {
class RenderWindow {
 public:
  virtual ~RenderWindow() = default;

  void InitRenderSurface();

  virtual Vector2 GetViewportSize() const = 0;
  virtual void SetupImGui() = 0;
  virtual void ImGuiNewFrame() = 0;
  virtual void DestroyImGui() = 0;
  virtual void UpdateImGuiPlatformWindows() = 0;
  virtual void SetupOpenglContext(int majorVersion, int minorVersion) = 0;
  virtual void* GetWindowOpenGLProcAddress() = 0;
  virtual const DynamicArray<const char*> GetVulkanWindowExtensions() = 0;
  virtual void* GetVulkanWindowSurface() = 0;

  static RendererAPI::API ChooseRenderingAPI(bool vulkanAvailable);

  template <typename T>
  ReferencePointer<T> GetSurfaceAttachment() const {
    static_assert(std::is_base_of<SurfaceAttachment, T>::value, "T must be derived from SurfaceAttachment");
    return std::dynamic_pointer_cast<T>(m_SurfaceAttachment);
  }

 private:
  ReferencePointer<SurfaceAttachment> m_SurfaceAttachment;
};
}  // namespace Hydrogen
