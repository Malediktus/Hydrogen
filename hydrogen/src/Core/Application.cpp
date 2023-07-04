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
  AppWindow = Window::Create(ApplicationInfo.Name, ApplicationInfo.WindowSize.x,
                             ApplicationInfo.WindowSize.y);

  ProjectInformation clientProject;
  clientProject.ProjectName = ApplicationInfo.Name;
  clientProject.ProjectVersion = ApplicationInfo.Version;

  Hydrogen::ProjectInformation engineProject;
  engineProject.ProjectName = "Hydrogen Engine";
  engineProject.ProjectVersion = glm::vec3(1, 0, 0);

  m_RenderContext = Context::Create(AppWindow);
  Renderer::SetContext(m_RenderContext);
  m_RenderContext->Init(clientProject, engineProject);

  RenderCommand::Init();
  RenderCommand::ConfigureAntiAliasing(true);

  AssetManager::Init();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad;             // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport
                                                       // / Platform Windows
  // io.ConfigViewportsNoAutoMerge = true;
  // io.ConfigViewportsNoTaskBarIcon = true;

  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  AppWindow->SetupImGui();
  RenderCommand::GetRendererAPI()->SetupImGui();

  // ReferencePointer<ShaderAsset> defaultShader =
  // AssetManager::Get<ShaderAsset>("assets/BlinnPhong.glsl");

  CurrentScene = NewReferencePointer<Scene>();

  // Register event callbacks
  EventDispatcher::Subscribe<WindowResizeEvent>(
      std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<WindowCloseEvent>(
      std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<KeyPressEvent>(
      std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<KeyReleaseEvent>(
      std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<KeyRepeatEvent>(
      std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<MouseMoveEvent>(
      std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<MousePressEvent>(
      std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<MouseReleaseEvent>(
      std::bind(&Application::OnEvent, this, std::placeholders::_1));
  EventDispatcher::Subscribe<MouseScrollEvent>(
      std::bind(&Application::OnEvent, this, std::placeholders::_1));

  EventDispatcher::Subscribe<WindowResizeEvent>(
      std::bind(&Application::OnResize, this, std::placeholders::_1));

  float planeVertices[] = {
      10.0f, -0.5f, 10.0f,  0.0f,  1.0f,   0.0f,  10.0f,  0.0f,  -10.0f, -0.5f,
      10.0f, 0.0f,  1.0f,   0.0f,  0.0f,   0.0f,  -10.0f, -0.5f, -10.0f, 0.0f,
      1.0f,  0.0f,  0.0f,   10.0f, 10.0f,  -0.5f, 10.0f,  0.0f,  1.0f,   0.0f,
      10.0f, 0.0f,  -10.0f, -0.5f, -10.0f, 0.0f,  1.0f,   0.0f,  0.0f,   10.0f,
      10.0f, -0.5f, -10.0f, 0.0f,  1.0f,   0.0f,  10.0f,  10.0f};

  uint32_t planeIndices[] = {0, 1, 2, 3, 4, 5};

  auto vertexBuffer =
      VertexBuffer::Create(planeVertices, sizeof(planeVertices));
  vertexBuffer->SetLayout({{ShaderDataType::Float3, "Position", false},
                           {ShaderDataType::Float3, "Normal", false},
                           {ShaderDataType::Float2, "TexCoord", false}});
  auto indexBuffer = IndexBuffer::Create(planeIndices, sizeof(planeIndices));
  auto vertexArray = VertexArray::Create();
  vertexArray->AddVertexBuffer(vertexBuffer);
  vertexArray->SetIndexBuffer(indexBuffer);

  // Vector3 lightPositions[] = {Vector3(-3.0f, 0.0f, 0.0f), Vector3(-1.0f,
  // 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(3.0f, 0.0f, 0.0f)}; Vector3
  // lightColors[] = {Vector3(0.25), Vector3(0.50), Vector3(0.75),
  // Vector3(1.00)};

  auto camera = NewReferencePointer<Camera>(90.0f, 1280.0f, 720.0f);
  camera->Translate(Vector3(0.0f, 3.0f, 12.0f));

  m_Initialized = true;
  OnInit();

  while (!AppWindow->GetWindowClose()) {
    TaskManager::Update();
    OnUpdate();

    RenderCommand::Clear(RendererAPI::ClearBuffer::COLOR);
    RenderCommand::Clear(RendererAPI::ClearBuffer::DEPTH);

    // m_WindowRenderer->BeginFrame(camera);
    // m_WindowRenderer->Submit(vertexArray);
    // for (uint32_t i = 0; i < 4; i++) {
    //     m_WindowRenderer->Submit(PointLight(1.0f, 1.0f, 1.0f, lightColors[i],
    //     lightColors[i], lightColors[i]), glm::translate(glm::mat4(1.0f),
    //     lightPositions[i]));
    // }
    // m_WindowRenderer->EndFrame();

    RenderCommand::GetRendererAPI()->ImGuiNewFrame();
    AppWindow->ImGuiNewFrame();
    ImGui::NewFrame();

    OnImGuiDraw();

    ImGui::Render();
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
  HY_LOG_DEBUG("Resizing renderer: {}, {}", resizeEvent.GetWidth(),
               resizeEvent.GetHeight());
  // m_WindowRenderer->OnResize(resizeEvent.GetWidth(),
  // resizeEvent.GetHeight());
}
