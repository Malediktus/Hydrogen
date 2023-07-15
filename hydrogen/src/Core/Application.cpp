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
  AppWindow = Window::Create(ApplicationInfo.Name, ApplicationInfo.WindowSize.x, ApplicationInfo.WindowSize.y);
  auto testWindow = Window::Create("Test", 500, 500);

  AssetManager::Init();

  ProjectInformation clientProject;
  clientProject.ProjectName = ApplicationInfo.Name;
  clientProject.ProjectVersion = ApplicationInfo.Version;

  Hydrogen::ProjectInformation engineProject;
  engineProject.ProjectName = "Hydrogen Engine";
  engineProject.ProjectVersion = glm::vec3(1, 0, 0);

  m_RenderContext = Context::Create(AppWindow);
  Renderer::SetContext(m_RenderContext);
  m_RenderContext->Init(clientProject, engineProject);

  m_RenderDevice = RenderDevice::Create([](const RenderDeviceProperties& deviceProperties) -> std::size_t {
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

  m_SwapChain = SwapChain::Create(m_RenderDevice, true);
  m_RenderPass = RenderPass::Create(m_RenderDevice, m_SwapChain);
  auto shaderAsset = AssetManager::Get<ShaderAsset>("assets/Raw.glsl");
  auto shader = Shader::Create(m_RenderDevice, m_SwapChain, m_RenderPass, shaderAsset->GetName(), shaderAsset->GetVertexShader(), shaderAsset->GetPixelShader(),
                               shaderAsset->GetGeometryShader());

  RenderCommand::Init();
  RenderCommand::ConfigureAntiAliasing(true);

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
  RenderCommand::GetRendererAPI()->SetupImGui();

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
    TaskManager::Update();
    OnUpdate();
    if (testWindow != nullptr && testWindow->GetWindowClose()) testWindow.reset();
    AppWindow->ImGuiNewFrame();
    // ImGui::NewFrame();
    OnImGuiDraw();
    // ImGui::Render();
    RenderCommand::GetRendererAPI()->ImGuiRenderDrawData(ImGui::GetDrawData());

    AppWindow->UpdateImGuiPlatformWindows();

    AppWindow->Render();
    AppWindow->UpdateEvents();
  }

  OnShutdown();

  RenderCommand::GetRendererAPI()->DestroyImGui();
  AppWindow->DestroyImGui();
  ImGui::DestroyContext();

  TaskManager::Shutdown();
  Renderer::SetContext(nullptr);
}

void Application::OnResize(const Event& event) {
  if (!m_Initialized) return;
  auto resizeEvent = dynamic_cast<const WindowResizeEvent&>(event);
  HY_LOG_DEBUG("Resizing renderer: {}, {}", resizeEvent.GetWidth(), resizeEvent.GetHeight());
}
