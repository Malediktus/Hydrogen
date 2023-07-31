#pragma once

#include "../../Core/Window.hpp"
#include "../../Math/Math.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Hydrogen {
class WindowsWindow : public Window {
 public:
  WindowsWindow(const String& title, uint32_t width, uint32_t height);
  ~WindowsWindow();

  void SetTitle(const String& title) override;
  uint32_t GetWidth() const override;
  uint32_t GetHeight() const override;
  Vector2 GetViewportSize() const override;

  bool GetWindowClose() const override;
  bool GetKeyDown(KeyCode key) const override;
  bool GetKey(KeyCode key) const override;
  bool GetKeyUp(KeyCode key) const override;
  bool GetMouseKeyDown(KeyCode key) const override;
  bool GetMouseKey(KeyCode key) const override;
  bool GetMouseKeyUp(KeyCode key) const override;
  uint32_t GetMouseX() const override;
  uint32_t GetMouseY() const override;
  Vector2 GetMousePos() const override;

  void UpdateEvents() override;
  void Render() override;

  void SetupImGui() override;
  void ImGuiNewFrame() override;
  void SetupOpenglContext(int major, int minor) override;
  void DestroyImGui() override;
  void UpdateImGuiPlatformWindows() override;

  void* GetWindowOpenGLProcAddress() override;
  const DynamicArray<char*> GetVulkanWindowExtensions() override;
  void* GetVulkanWindowSurface() override;

 private:
  GLFWwindow* m_Window;
  static bool s_GlfwInitialized;
  void* m_VulkanSurface;
};
}  // namespace Hydrogen
