#include <Panel.hpp>
#include <Panels/AssetNavigatorPanel.hpp>

using namespace HydrogenEditor;

class EditorApp : public Hydrogen::Application {
 public:
  void OnSetup() override {
    ApplicationInfo.Name = "Hydrogen Editor";
    ApplicationInfo.Version = {1, 0, 0};
    ApplicationInfo.WindowSize = {1080, 720};
  }

  void OnInit() override {
    // AttachPanel(Hydrogen::NewReferencePointer<AssetNavigatorPanel>());
  }

  void AttachPanel(Hydrogen::ReferencePointer<Panel> panel) {
    Hydrogen::TaskManager::Activate(panel);
    m_Panels.push_back(panel);
  }

  void DetachPanel(Hydrogen::ReferencePointer<Panel> panel) {
    Hydrogen::TaskManager::Deactivate(panel);
    Hydrogen::DynamicArray<Hydrogen::ReferencePointer<Panel>>::iterator position = std::find(m_Panels.begin(), m_Panels.end(), panel);
    if (position != m_Panels.end()) m_Panels.erase(position);
  }

  void OnShutdown() override {}

  void OnUpdate() override {}

  void OnImGuiDraw() override {
    for (auto& panel : m_Panels) {
      ImGui::Begin(panel->GetTitle().c_str());
      panel->OnImGuiRender();
      ImGui::End();
    }
  }

 private:
  Hydrogen::ReferencePointer<Hydrogen::Task> m_DemoTask;
  Hydrogen::DynamicArray<Hydrogen::ReferencePointer<Panel>> m_Panels;
};

Hydrogen::ReferencePointer<Hydrogen::Application> Hydrogen::CreateApplication() {
  std::filesystem::current_path(WORKING_DIR);
  return Hydrogen::NewReferencePointer<EditorApp>();
}
