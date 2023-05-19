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

    ProjectInformation rendererProject;
    rendererProject.ProjectName = ApplicationInfo.Name;
    rendererProject.ProjectVersion = ApplicationInfo.Version;

    m_RenderContext = Context::Create(AppWindow, rendererProject);
    m_RenderContext->Init();

    Renderer::SetContext(m_RenderContext);
    RenderCommand::Init();
    RenderCommand::ConfigureAntiAliasing(true);

    AssetManager::Init();

    Reference<ShaderAsset> defaultShader = AssetManager::Get<ShaderAsset>("assets/BlinnPhong.glsl");
    m_WindowRenderer = NewReference<Renderer>(defaultShader->GetVortexShader(), 1280, 720, false);

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

        // m_WindowRenderer->BeginFrame(camera);
        // m_WindowRenderer->EndFrame();

        AppWindow->Render();
        AppWindow->UpdateEvents();
    }

    OnShutdown();
    TaskManager::Shutdown();
    m_RenderContext->Destroy();
}
