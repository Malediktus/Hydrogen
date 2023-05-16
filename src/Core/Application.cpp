#include <Hydrogen/Core/Application.hpp>
#include <Hydrogen/Core/Task.hpp>
#include <Hydrogen/Assets/AssetManager.hpp>

using namespace Hydrogen;

Application::Application() {
    Console = NewReference<Logger>("APP", Logger::LogLevel::Debug);
    SystemLogger::Init();
}

void Application::Run() {
    AssetManager::Init();
    OnSetup();
    AppWindow = Window::Create(ApplicationInfo.Name, ApplicationInfo.WindowSize.x, ApplicationInfo.WindowSize.y);
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
    }

    OnShutdown();
    TaskManager::Shutdown();
}
