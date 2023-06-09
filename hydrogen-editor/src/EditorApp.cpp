#include <Hydrogen/Hydrogen.hpp>

namespace HydrogenEditor {
class Panel : public Hydrogen::Task {
public:
    virtual ~Panel() = default;

    virtual void OnImGuiRender() = 0;
    virtual const Hydrogen::String& GetTitle() = 0;
};

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

    void AttachPanel(Hydrogen::ReferencePointer<Panel> panel) {
        Hydrogen::TaskManager::Activate(panel);
        m_Panels.push_back(panel);
    }

    void DetachPanel(Hydrogen::ReferencePointer<Panel> panel) {
        Hydrogen::TaskManager::Deactivate(panel);
        Hydrogen::DynamicArray<Hydrogen::ReferencePointer<Panel>>::iterator position = std::find(m_Panels.begin(), m_Panels.end(), panel);
        if (position != m_Panels.end())
            m_Panels.erase(position);
    }

    void OnShutdown() override {
    }

    void OnUpdate() override {
    }

    void OnImGuiDraw() override {
        for (auto& panel : m_Panels) {
            ImGui::Begin(panel->GetTitle().c_str());
            panel->OnImGuiRender();
            ImGui::End();
        }
    }

    void OnEvent(const Hydrogen::Event&) override {
    }

private:
    Hydrogen::ReferencePointer<Hydrogen::Task> m_DemoTask;
    Hydrogen::DynamicArray<Hydrogen::ReferencePointer<Panel>> m_Panels;
};
} // namespace HydrogenEditor

Hydrogen::ReferencePointer<Hydrogen::Application> Hydrogen::CreateApplication() {
    return Hydrogen::NewReferencePointer<HydrogenEditor::EditorApp>();
}
