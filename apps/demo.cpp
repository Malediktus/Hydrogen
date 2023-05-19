#include <Hydrogen/Hydrogen.hpp>

class DemoTask : public Hydrogen::Task {
public:
    DemoTask(Hydrogen::Reference<Hydrogen::Logger> console) {
        Console = console;
    }

    void OnActivate() override {
        Console->Debug("Activate");
    }

    void OnUpdate() override {
    }

    void OnDeactivate() override {
        Console->Debug("Deactivate");
    }

private:
    Hydrogen::Reference<Hydrogen::Logger> Console;
};

class DemoApplication : public Hydrogen::Application {
public:
    void OnSetup() override {
        ApplicationInfo.Name = "Hydrogen Demo";
        ApplicationInfo.Version = {1, 0, 0};
        ApplicationInfo.WindowSize = {1080, 720};
    }

    void OnInit() override {
        Console->Debug("Started App");
        m_DemoTask = Hydrogen::TaskManager::Activate(Hydrogen::NewReference<DemoTask>(Console));

        auto entity = CurrentScene->AddEntity("PointLight");
        entity->AddComponent<Hydrogen::TransformComponent>(glm::vec3(-3.0f, 0.0f, 0.0f));
        entity->AddComponent<Hydrogen::PointLightComponent>(Vortex::PointLight(1.0f, 1.0f, 1.0f, glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)));
    }

    void OnShutdown() override {
    }

    void OnUpdate() override {
    }

    void OnEvent(const Hydrogen::Event&) override {
        Console->Debug("An event occured");
    }

private:
    Hydrogen::Reference<Hydrogen::Task> m_DemoTask;
};

Hydrogen::Reference<Hydrogen::Application> Hydrogen::CreateApplication() {
    return Hydrogen::NewReference<DemoApplication>();
}
