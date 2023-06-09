#include <Hydrogen/Hydrogen.hpp>

class RenderTask : public Hydrogen::Task {
public:
    RenderTask() {
    }

    ~RenderTask() = default;

    void OnActivate() override {
    }

    void OnUpdate() override {
    }

    void OnDeactivate() override {
    }
};

class EditorApp : public Hydrogen::Application {
public:
    void OnSetup() override {
        ApplicationInfo.Name = "Hydrogen Editor";
        ApplicationInfo.Version = {1, 0, 0};
        ApplicationInfo.WindowSize = {1080, 720};
    }

    void OnInit() override {
        Hydrogen::TaskManager::Activate(Hydrogen::NewReferencePointer<RenderTask>());
    }

    void OnShutdown() override {
    }

    void OnUpdate() override {
    }

    void OnEvent(const Hydrogen::Event&) override {
    }

private:
    Hydrogen::ReferencePointer<Hydrogen::Task> m_DemoTask;
};

Hydrogen::ReferencePointer<Hydrogen::Application> Hydrogen::CreateApplication() {
    return Hydrogen::NewReferencePointer<EditorApp>();
}
