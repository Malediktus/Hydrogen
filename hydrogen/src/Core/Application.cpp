#include <imgui.h>

#include <Hydrogen/Assets/AssetManager.hpp>
#include <Hydrogen/Core/Application.hpp>
#include <Hydrogen/Core/Task.hpp>

using namespace Hydrogen;

Application::Application() {
  Console = NewReferencePointer<Logger>("APP", Logger::LogLevel::Info);
  SystemLogger::Init();
}

void Application::Run() {
  OnSetup();
  AppWindow = Window::Create(ApplicationInfo.Name, static_cast<uint32_t>(ApplicationInfo.WindowSize.x), static_cast<uint32_t>(ApplicationInfo.WindowSize.y));
  auto popup = Window::Create("Popup", 500, 500);

  AssetManager::Init();

  ProjectInformation clientProject;
  clientProject.ProjectName = ApplicationInfo.Name;
  clientProject.ProjectVersion = ApplicationInfo.Version;

  ProjectInformation engineProject;
  engineProject.ProjectName = "Hydrogen Engine";
  engineProject.ProjectVersion = glm::vec3(1, 0, 0);

  auto renderContext = Context::Create(AppWindow);
  Renderer::SetContext(renderContext);
  renderContext->Init(clientProject, engineProject);

  auto renderDevice = RenderDevice::Create([](const RenderDeviceProperties& deviceProperties) -> std::size_t {
    size_t result = 0;

    switch (deviceProperties.DeviceType) {
      case RenderDeviceType::IntegratedGPU:
        break;
      case RenderDeviceType::DiscreteGPU:
        result += 1000000;  // Always choose discrete GPUs
        break;
      default:
        return 0;  // Support only GPUs
    }

    for (auto heap : deviceProperties.MemoryHeaps) result += heap.MemorySize / 1024;  // Choose the GPU with most memory

    return result;
  });

  HY_ASSERT(!renderDevice->ScreenSupported(AppWindow), "Screen is not supported!");  // TODO: Choose other graphics API or device
  auto renderer = NewReferencePointer<Renderer>(AppWindow, renderDevice);

  HY_ASSERT(!renderDevice->ScreenSupported(popup), "Screen is not supported!");
  auto renderer2 = NewReferencePointer<Renderer>(popup, renderDevice);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport
                                                         // / Platform Windows

  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  AppWindow->SetupImGui();
  // RenderCommand::GetRendererAPI()->SetupImGui();

  CurrentScene = NewReferencePointer<Scene>();

  // Register event callbacks
  EventDispatcher::Subscribe<WindowResizeEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<WindowCloseEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<KeyPressEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<KeyReleaseEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<KeyRepeatEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<MouseMoveEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<MousePressEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<MouseReleaseEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<MouseScrollEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));

  EventDispatcher::Subscribe<WindowResizeEvent>(std::bind(&Application::OnResize, this, std::placeholders::_1));

  m_Initialized = true;
  OnInit();

  while (!AppWindow->GetWindowClose()) {
    if (popup != nullptr) {
      popup->Render();
      popup->UpdateEvents();
      renderer2->Render();

      if (popup->GetWindowClose()) {
        popup.reset();
        renderer2.reset();
      };
    }

    TaskManager::Update();
    OnUpdate();
    renderer->Render();

    AppWindow->ImGuiNewFrame();
    // ImGui::NewFrame();
    OnImGuiDraw();
    // ImGui::Render();
    // RenderCommand::GetRendererAPI()->ImGuiRenderDrawData(ImGui::GetDrawData());

    AppWindow->UpdateImGuiPlatformWindows();

    AppWindow->Render();
    AppWindow->UpdateEvents();
  }

  OnShutdown();

  // RenderCommand::GetRendererAPI()->DestroyImGui();
  AppWindow->DestroyImGui();
  ImGui::DestroyContext();

  renderDevice->WaitForIdle();

  TaskManager::Shutdown();
  if (popup != nullptr) renderer2.reset();
  renderer.reset();
  renderDevice.reset();
  Renderer::SetContext(nullptr);
}

void Application::OnResize(const Event& event) {
  if (!m_Initialized) return;
  auto resizeEvent = dynamic_cast<const WindowResizeEvent&>(event);
  HY_LOG_DEBUG("Resizing renderer: {}, {}", resizeEvent.GetWidth(), resizeEvent.GetHeight());
}
