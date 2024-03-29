#include <backends/imgui_impl_glfw.h>
#include <imgui.h>

#include <Hydrogen/Platform/MacOS/MacOSWindow.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <Hydrogen/Core/Base.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>

using namespace Hydrogen;

bool MacOSWindow::s_GlfwInitialized;

MacOSWindow::MacOSWindow(const std::string& title, uint32_t width, uint32_t height) {
  if (!s_GlfwInitialized) {
    HY_ASSERT(glfwInit(), "Init glfw");
    s_GlfwInitialized = true;
  }

  auto api = RenderWindow::ChooseRenderingAPI(glfwVulkanSupported());

  glfwWindowHint(GLFW_NO_API, GLFW_TRUE);
  if (api == RendererAPI::API::OpenGL) {
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  } else if (api == RendererAPI::API::Vulkan) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  } else {
    HY_INVOKE_ERROR("Unsupported graphics API");
  }

  m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  m_VulkanSurface = nullptr;
  HY_ASSERT(m_Window, "glfw window is null");
}

MacOSWindow::~MacOSWindow() { glfwDestroyWindow(m_Window); }

void MacOSWindow::SetTitle(const std::string& title) { glfwSetWindowTitle(m_Window, title.c_str()); }

uint32_t MacOSWindow::GetWidth() const {
  int width, height;
  glfwGetWindowSize(m_Window, &width, &height);
  return (uint32_t)width;
}

uint32_t MacOSWindow::GetHeight() const {
  int width, height;
  glfwGetWindowSize(m_Window, &width, &height);
  return (uint32_t)height;
}

Vector2 MacOSWindow::GetViewportSize() const {
  int width, height;
  glfwGetWindowSize(m_Window, &width, &height);
  return Vector2(width, height);
}

bool MacOSWindow::GetWindowClose() const { return glfwWindowShouldClose(m_Window); }

bool MacOSWindow::GetKeyDown(KeyCode key) const { return glfwGetKey(m_Window, (int)key) == GLFW_PRESS; }

bool MacOSWindow::GetKey(KeyCode key) const { return glfwGetKey(m_Window, (int)key) == GLFW_PRESS; }

bool MacOSWindow::GetKeyUp(KeyCode key) const { return glfwGetKey(m_Window, (int)key) == GLFW_RELEASE; }

bool MacOSWindow::GetMouseKeyDown(KeyCode key) const { return glfwGetMouseButton(m_Window, (int)key) == GLFW_PRESS; }

bool MacOSWindow::GetMouseKey(KeyCode key) const { return glfwGetMouseButton(m_Window, (int)key) == GLFW_PRESS; }

bool MacOSWindow::GetMouseKeyUp(KeyCode key) const { return glfwGetMouseButton(m_Window, (int)key) == GLFW_RELEASE; }

uint32_t MacOSWindow::GetMouseX() const {
  double x, y;
  glfwGetCursorPos(m_Window, &x, &y);
  return static_cast<uint32_t>(x);
}

uint32_t MacOSWindow::GetMouseY() const {
  double x, y;
  glfwGetCursorPos(m_Window, &x, &y);
  return static_cast<uint32_t>(y);
}

Vector2 MacOSWindow::GetMousePos() const {
  double x, y;
  glfwGetCursorPos(m_Window, &x, &y);
  return Vector2(x, y);
}

void MacOSWindow::UpdateEvents() { glfwPollEvents(); }

void MacOSWindow::Render() {
  glfwSwapBuffers(m_Window);

  // Only for MacOS (Because of some strange glfw bug)
  static bool moved = false;
  if (!moved) {
    int x, y;
    glfwGetWindowPos(m_Window, &x, &y);
    glfwSetWindowPos(m_Window, x + 1, y);
    glfwSetWindowPos(m_Window, x, y);
    moved = true;
  }
}

void MacOSWindow::SetupImGui() {
  ImGui_ImplGlfw_InitForVulkan(m_Window, true);  // TODO: Make API indenpendent
}

void MacOSWindow::ImGuiNewFrame() {
  ImGui_ImplGlfw_NewFrame();
}

void MacOSWindow::DestroyImGui() {
  ImGui_ImplGlfw_Shutdown();
}

void MacOSWindow::SetupOpenglContext(int, int) { glfwMakeContextCurrent(m_Window); }

void* MacOSWindow::GetWindowOpenGLProcAddress() { return (void*)glfwGetProcAddress; }

void MacOSWindow::UpdateImGuiPlatformWindows() {
  // if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
  //   GLFWwindow* backup_current_context = glfwGetCurrentContext();
  //   ImGui::UpdatePlatformWindows();
  //   ImGui::RenderPlatformWindowsDefault();
  //   glfwMakeContextCurrent(backup_current_context);
  // }
}

const DynamicArray<char*> MacOSWindow::GetVulkanWindowExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  DynamicArray<char*> result;
  result.reserve(glfwExtensionCount);
  for (uint32_t i = 0; i < glfwExtensionCount; i++) {
    result.push_back((char*)glfwExtensions[i]);
  }

  return result;
}

void* MacOSWindow::GetVulkanWindowSurface() {
  if (m_VulkanSurface != nullptr) {
    return m_VulkanSurface;
  }

  VkSurfaceKHR surface;
  VkInstance instance = Renderer::GetContext<Vulkan::VulkanContext>()->GetInstance();
  VK_CHECK_ERROR(glfwCreateWindowSurface(instance, m_Window, NULL, &surface), "Failed to create window surface");
  m_VulkanSurface = (void*)surface;
  return m_VulkanSurface;
}
