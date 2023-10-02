#include <Panel.hpp>
#include <Panels/AssetNavigatorPanel.hpp>

using namespace HydrogenEditor;

void EditorApp::OnSetup() {
  ApplicationInfo.Name = "Hydrogen Editor";
  ApplicationInfo.Version = {1, 0, 0};
  ApplicationInfo.WindowSize = {1080, 720};
}

void EditorApp::OnInit() { AttachPanel(Hydrogen::NewReferencePointer<AssetNavigatorPanel>(this)); }

void EditorApp::AttachPanel(Hydrogen::ReferencePointer<Panel> panel) {
  Hydrogen::TaskManager::Activate(panel);
  m_Panels.push_back(panel);
}

void EditorApp::DetachPanel(Hydrogen::ReferencePointer<Panel> panel) {
  Hydrogen::TaskManager::Deactivate(panel);
  Hydrogen::DynamicArray<Hydrogen::ReferencePointer<Panel>>::iterator position = std::find(m_Panels.begin(), m_Panels.end(), panel);
  if (position != m_Panels.end()) m_Panels.erase(position);
}

void EditorApp::OnShutdown() {}

void EditorApp::OnUpdate() {}

void EditorApp::OnImGuiDraw() {
  for (auto& panel : m_Panels) {
    ImGui::Begin(panel->GetTitle().c_str());
    panel->OnImGuiRender();
    ImGui::End();
  }
}

Hydrogen::ReferencePointer<Hydrogen::Application> Hydrogen::CreateApplication() {
  std::filesystem::current_path(WORKING_DIR);
  return Hydrogen::NewReferencePointer<EditorApp>();
}
