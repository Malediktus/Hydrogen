#include <imgui.h>

#include <Hydrogen/Assets/AssetManager.hpp>
#include <Hydrogen/Core/Application.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Core/Task.hpp>
#include <Hydrogen/Core/Window.hpp>
#include <Hydrogen/Renderer/Context.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/VertexArray.hpp>
#include <Hydrogen/Scene/Scene.hpp>

using namespace Hydrogen;

// TODO: Remove
void ProcessMeshNode(const ReferencePointer<MeshNode>& node, const ReferencePointer<Renderer>& renderer) {
  for (auto& mesh : node->Meshes) {
    renderer->Submit(mesh.VertexArray, nullptr, node->Transform);
  }

  for (const auto& child : node->Chilren) {
    ProcessMeshNode(child, renderer);
  }
}

Application::Application() {
  Console = NewReferencePointer<Logger>("APP", Logger::LogLevel::Info);
  SystemLogger::Init();
}

void Application::Run() {
  OnSetup();
  AppWindow = Window::Create(ApplicationInfo.Name, static_cast<uint32_t>(ApplicationInfo.WindowSize.x), static_cast<uint32_t>(ApplicationInfo.WindowSize.y));

  ProjectInformation clientProject;
  clientProject.ProjectName = ApplicationInfo.Name;
  clientProject.ProjectVersion = ApplicationInfo.Version;

  ProjectInformation engineProject;
  engineProject.ProjectName = "Hydrogen Engine";
  engineProject.ProjectVersion = glm::vec3(1, 0, 0);

  auto renderContext = Context::Create(AppWindow);
  Renderer::SetContext(renderContext);
  renderContext->Init(clientProject, engineProject);

  Renderer::SetRenderDevice(RenderDevice::Create([](const RenderDeviceProperties& deviceProperties) -> std::size_t {
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
  }));

  AssetManager::Init();

  AppWindow->InitRenderSurface();

  CurrentScene = NewScopePointer<Scene>("Main Scene");

  auto rendererAPI = RendererAPI::Create(AppWindow); // TODO: Currently a singleton, means only one window is supported

  HY_ASSERT(!Renderer::GetRenderDevice<RenderDevice>()->ScreenSupported(AppWindow), "Screen is not supported!");  // TODO: Choose other graphics API or device
  auto renderer = NewReferencePointer<Renderer>(AppWindow);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking

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

    renderer->BeginFrame();
    const auto& meshTree = AssetManager::Get<MeshAsset>("assets/Meshes/Backpack/Survival_BackPack_2.fbx")->GetTree();
    ProcessMeshNode(meshTree, renderer);
    renderer->EndFrame();

    AppWindow->UpdateImGuiPlatformWindows();

    AppWindow->Render();
    AppWindow->UpdateEvents();
  }

  OnShutdown();

  Renderer::GetRenderDevice<RenderDevice>()->WaitForIdle();

  rendererAPI->DestroyImGui();
  AppWindow->DestroyImGui();
  ImGui::DestroyContext();

  TaskManager::Shutdown();
}
