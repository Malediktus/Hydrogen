#include <Hydrogen/Core/Application.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Core/Window.hpp>
#include <Hydrogen/Core/Task.hpp>
#include <Hydrogen/Assets/AssetManager.hpp>
#include <Hydrogen/Renderer/Context.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/Framebuffer.hpp>
#include <Hydrogen/Scene/Scene.hpp>
#include <imgui.h>

using namespace Hydrogen;

Application::Application() {
  Console = NewReferencePointer<Logger>("APP", Logger::LogLevel::Info);
  SystemLogger::Init();
}

void Application::Run() {
  OnSetup();
  AppWindow = Window::Create(ApplicationInfo.Name, static_cast<uint32_t>(ApplicationInfo.WindowSize.x), static_cast<uint32_t>(ApplicationInfo.WindowSize.y));

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

  MainRenderDevice = RenderDevice::Create([](const RenderDeviceProperties& deviceProperties) -> std::size_t {
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

    for (auto& heap : deviceProperties.MemoryHeaps) result += heap.MemorySize / 1024;  // Choose the GPU with most memory

    return result;
  });

  CurrentScene = NewScopePointer<Scene>("Main Scene");

  auto test = AssetManager::Get<MeshAsset>("assets/Meshes/Monkey/monkey.fbx");
  test->Spawn(MainRenderDevice, CurrentScene, "Backpack");

  HY_ASSERT(!MainRenderDevice->ScreenSupported(AppWindow), "Screen is not supported!");  // TODO: Choose other graphics API or device
  auto renderer = NewReferencePointer<Renderer>(AppWindow, MainRenderDevice, CurrentScene);

  auto rendererAPI = RendererAPI::Create(MainRenderDevice, renderer->GetFramebuffer());

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport (Unimplemented  in vulkan)

  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  AppWindow->SetupImGui();
  rendererAPI->SetupImGui();

  m_Initialized = true;
  OnInit();

  while (!AppWindow->GetWindowClose()) {
    TaskManager::Update();
    OnUpdate();

    AppWindow->ImGuiNewFrame();
    rendererAPI->ImGuiNewFrame();
    ImGui::NewFrame();
    OnImGuiDraw();
    ImGui::Render();

    renderer->Render(); // Also renders imgui draw data

    AppWindow->UpdateImGuiPlatformWindows();

    AppWindow->Render();
    AppWindow->UpdateEvents();
  }

  OnShutdown();

  MainRenderDevice->WaitForIdle();

  rendererAPI->DestroyImGui();
  AppWindow->DestroyImGui();
  ImGui::DestroyContext();

  //renderer.reset();
  //MainRenderDevice.reset();
  //Renderer::SetContext(nullptr);

  TaskManager::Shutdown();
}
