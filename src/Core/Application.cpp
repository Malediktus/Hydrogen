#include <Hydrogen/Core/Application.hpp>

using namespace Hydrogen;

Application::Application() {
    Console = NewReference<Logger>("APP", Logger::LogLevel::Debug);
}

void Application::Run() {
    OnInit();
    while (true) {
        OnUpdate();
    }
    OnShutdown();
}
