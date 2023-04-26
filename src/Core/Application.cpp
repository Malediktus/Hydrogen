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
    OnInit();
    EventDispatcher::Subscribe<WindowResizeEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<WindowCloseEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<KeyPressEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<KeyReleaseEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<KeyRepeatEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<MouseMoveEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<MousePressEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<MouseReleaseEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<MouseScrollEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    // while (true) {
    TaskManager::Update();
    OnUpdate();
    //}
    OnShutdown();
    TaskManager::Shutdown();
}
