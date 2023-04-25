#include <Hydrogen/Core/Application.hpp>
#include <Hydrogen/Core/Task.hpp>

using namespace Hydrogen;

Application::Application() {
    Console = NewReference<Logger>("APP", Logger::LogLevel::Debug);
    SystemLogger::Init();
}

void Application::Run() {
    OnInit();
    // while (true) {
    TaskManager::Update();
    OnUpdate();
    //}
    OnShutdown();
    TaskManager::Shutdown();
}
