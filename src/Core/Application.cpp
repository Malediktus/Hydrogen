#include <Hydrogen/Core/Application.hpp>
#include <Hydrogen/Core/Task.hpp>
#include <Hydrogen/Assets/Manager.hpp>

using namespace Hydrogen;

Application::Application() {
    Console = NewReference<Logger>("APP", Logger::LogLevel::Debug);
    SystemLogger::Init();
}

void Application::Run() {
    AssetManager::Init();
    OnInit();
    // while (true) {
    TaskManager::Update();
    OnUpdate();
    //}
    OnShutdown();
    TaskManager::Shutdown();
}
