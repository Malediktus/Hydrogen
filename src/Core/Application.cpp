#include <Hydrogen/Core/Application.hpp>
#include <Hydrogen/Core/Task.hpp>

using namespace Hydrogen;

Application::Application() {
    Console = NewReference<Logger>("APP", Logger::LogLevel::Debug);
    SystemLogger::Init();
}

class TestTask : public Task {
    void OnActivate() override {
        HY_LOG_INFO("Activate");
    }
    void OnUpdate() override {
        HY_LOG_INFO("Update");
    }
    void OnDeactivate() override {
        HY_LOG_INFO("Deactivate");
    }
};

void Application::Run() {
    auto task = TaskManager::Activate(NewReference<TestTask>());
    OnInit();
    // while (true) {
    TaskManager::Update();
    TaskManager::Deactivate(task);
    OnUpdate();
    //}
    OnShutdown();
}
