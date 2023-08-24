#include <backends/imgui_impl_glfw.h>
#include <imgui.h>

#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Windows/WindowsWindow.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>

using namespace Hydrogen;

Window* Window::s_MainWindow = nullptr;

WindowsWindow::WindowsWindow(const std::string& title, uint32_t width, uint32_t height) {
  if (s_MainWindow == nullptr) {
    HY_ASSERT(glfwInit(), "Failed to initialize GLFW!");
    s_MainWindow = this;
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

WindowsWindow::~WindowsWindow() {
  glfwDestroyWindow(m_Window);
  if (s_MainWindow == this) {
    glfwTerminate();
  }
}

void WindowsWindow::SetTitle(const std::string& title) { glfwSetWindowTitle(m_Window, title.c_str()); }

uint32_t WindowsWindow::GetWidth() const {
  int width, height;
  glfwGetWindowSize(m_Window, &width, &height);
  return (uint32_t)width;
}

uint32_t WindowsWindow::GetHeight() const {
  int width, height;
  glfwGetWindowSize(m_Window, &width, &height);
  return (uint32_t)height;
}

Vector2 WindowsWindow::GetViewportSize() const {
  int width, height;
  glfwGetWindowSize(m_Window, &width, &height);
  return Vector2(width, height);
}

bool WindowsWindow::GetWindowClose() const { return glfwWindowShouldClose(m_Window); }

bool WindowsWindow::GetKeyDown(KeyCode key) const { return glfwGetKey(m_Window, (int)key) == GLFW_PRESS; }

bool WindowsWindow::GetKey(KeyCode key) const { return glfwGetKey(m_Window, (int)key) == GLFW_PRESS; }

bool WindowsWindow::GetKeyUp(KeyCode key) const { return glfwGetKey(m_Window, (int)key) == GLFW_RELEASE; }

bool WindowsWindow::GetMouseKeyDown(KeyCode key) const { return glfwGetMouseButton(m_Window, (int)key) == GLFW_PRESS; }

bool WindowsWindow::GetMouseKey(KeyCode key) const { return glfwGetMouseButton(m_Window, (int)key) == GLFW_PRESS; }

bool WindowsWindow::GetMouseKeyUp(KeyCode key) const { return glfwGetMouseButton(m_Window, (int)key) == GLFW_RELEASE; }

uint32_t WindowsWindow::GetMouseX() const {
  double x, y;
  glfwGetCursorPos(m_Window, &x, &y);
  return static_cast<uint32_t>(x);
}

uint32_t WindowsWindow::GetMouseY() const {
  double x, y;
  glfwGetCursorPos(m_Window, &x, &y);
  return static_cast<uint32_t>(y);
}

Vector2 WindowsWindow::GetMousePos() const {
  double x, y;
  glfwGetCursorPos(m_Window, &x, &y);
  return Vector2(x, y);
}

void WindowsWindow::UpdateEvents() { glfwPollEvents(); }

void WindowsWindow::Render() {
  glfwSwapBuffers(m_Window);
}

void WindowsWindow::SetupImGui() {
  // ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
}

void WindowsWindow::ImGuiNewFrame() {
  // ImGui_ImplGlfw_NewFrame();
}

void WindowsWindow::DestroyImGui() {
  // ImGui_ImplGlfw_Shutdown();
}

void WindowsWindow::SetupOpenglContext(int, int) { glfwMakeContextCurrent(m_Window); }

void* WindowsWindow::GetWindowOpenGLProcAddress() { return (void*)glfwGetProcAddress; }

void WindowsWindow::UpdateImGuiPlatformWindows() {
  // if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
  //   GLFWwindow* backup_current_context = glfwGetCurrentContext();
  //   ImGui::UpdatePlatformWindows();
  //   ImGui::RenderPlatformWindowsDefault();
  //   glfwMakeContextCurrent(backup_current_context);
  // }
}

const DynamicArray<char*> WindowsWindow::GetVulkanWindowExtensions() {
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

void* WindowsWindow::GetVulkanWindowSurface() {
  if (m_VulkanSurface != nullptr) {
    return m_VulkanSurface;
  }

  VkSurfaceKHR surface;
  VkInstance instance = Renderer::GetContext<Vulkan::VulkanContext>()->GetInstance();
  VK_CHECK_ERROR(glfwCreateWindowSurface(instance, m_Window, NULL, &surface), "Failed to create window surface");
  m_VulkanSurface = (void*)surface;
  return m_VulkanSurface;
}
