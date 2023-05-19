#include <Hydrogen/Core/Application.hpp>
#include <Hydrogen/Core/Task.hpp>
#include <Hydrogen/Assets/AssetManager.hpp>

using namespace Hydrogen;

Application::Application() {
    Console = NewReference<Logger>("APP", Logger::LogLevel::Debug);
    SystemLogger::Init();
}

void Application::Run() {
    OnSetup();
    AppWindow = Window::Create(ApplicationInfo.Name, ApplicationInfo.WindowSize.x, ApplicationInfo.WindowSize.y);

    Vortex::ProjectInformation vortexProject;
    vortexProject.ProjectName = ApplicationInfo.Name;
    vortexProject.ProjectVersion = ApplicationInfo.Version;

    m_VortexContext = Vortex::ContextCreate(AppWindow, vortexProject);
    m_VortexContext->Init();

    Vortex::Renderer::SetContext(m_VortexContext);
    Vortex::RenderCommand::Init();
    Vortex::RenderCommand::ConfigureAntiAliasing(true);

    AssetManager::Init();

    Reference<ShaderAsset> defaultShader = AssetManager::Get<ShaderAsset>("assets/BlinnPhong.glsl");
    m_WindowRenderer = NewReference<Vortex::Renderer>(defaultShader->GetVortexShader(), 1280, 720, false);

    CurrentScene = NewReference<Scene>();

    OnInit();
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

    while (!AppWindow->GetWindowClose()) {
        TaskManager::Update();
        OnUpdate();
        m_WindowRenderer->BeginFrame();
        m_WindowRenderer->EndFrame();
        AppWindow->Render();
        AppWindow->UpdateEvents();
    }

    OnShutdown();
    TaskManager::Shutdown();
    m_VortexContext->Destroy();
}
