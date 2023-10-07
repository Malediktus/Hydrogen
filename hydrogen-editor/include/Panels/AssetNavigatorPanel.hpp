#pragma once

#include "../Panel.hpp"

namespace HydrogenEditor {
class EditorApp : public Hydrogen::Application {
 public:
  void OnSetup() override;
  void OnInit() override;
  void AttachPanel(Hydrogen::ReferencePointer<Panel> panel);
  void DetachPanel(Hydrogen::ReferencePointer<Panel> panel);
  void OnShutdown() override;
  void OnUpdate() override;
  void OnImGuiDraw() override;

 private:
  Hydrogen::ReferencePointer<Hydrogen::Task> m_DemoTask;
  Hydrogen::DynamicArray<Hydrogen::ReferencePointer<Panel>> m_Panels;
};

class AssetViewPanel : public Panel {
 public:
  AssetViewPanel(EditorApp* app, const std::filesystem::path& filepath) : m_App(app), m_Filepath(filepath) {}
  ~AssetViewPanel() = default;

  virtual void OnImGuiRender() override {
    if (ImGui::Button("X")) {
      m_CloseEvent.Invoke();
    }
  }

  virtual const Hydrogen::String GetTitle() override { return m_Filepath.filename().string(); }

  virtual void OnActivate() override {}

  virtual void OnUpdate() override {}

  virtual void OnDeactivate() override {}

  Hydrogen::HydrogenEvent<>& GetCloseEvent() { return m_CloseEvent; }

 private:
  EditorApp* m_App;
  Hydrogen::HydrogenEvent<> m_CloseEvent;
  const std::filesystem::path m_Filepath;
};

class AssetNavigatorPanel : public Panel {
 public:
  AssetNavigatorPanel(EditorApp* app) : m_App(app) {}
  ~AssetNavigatorPanel() = default;

  void OnAssetViewPanelClose() {
    m_App->DetachPanel(m_CurrentAssetPanel);
    m_CurrentAssetPanel = nullptr;
  }

  virtual void OnImGuiRender() override {
    if (m_CurrentDirectory != std::filesystem::path("assets")) {
      if (ImGui::Button("<-")) {
        m_CurrentDirectory = m_CurrentDirectory.parent_path();
      }
    }

    for (auto& directoryEntry :
    std::filesystem::directory_iterator(m_CurrentDirectory)) {
      const auto& path = directoryEntry.path();
      auto relativePath = std::filesystem::relative(path, "assets");
      std::string filenameString = relativePath.filename().string();
      if (directoryEntry.is_directory()) {
        if (ImGui::Button(filenameString.c_str())) {
          m_CurrentDirectory /= path.filename();
        }
      } else if (directoryEntry.is_regular_file()) {
        if (ImGui::Button(filenameString.c_str())) {
          if (m_CurrentAssetPanel != nullptr) {
            m_App->DetachPanel(m_CurrentAssetPanel);
          }

          auto assetViewPanel = Hydrogen::NewReferencePointer<AssetViewPanel>(m_App, directoryEntry.path());
          assetViewPanel->GetCloseEvent().RegisterCallback(std::bind(&AssetNavigatorPanel::OnAssetViewPanelClose, this));
          m_CurrentAssetPanel = assetViewPanel;
          m_App->AttachPanel(m_CurrentAssetPanel);
        }
      }
    }
  }

  virtual const Hydrogen::String GetTitle() override { return "Asset Navigator"; }

  virtual void OnActivate() override {
    m_CurrentDirectory = std::filesystem::path("assets");
    m_CurrentAssetPanel = nullptr;
  }

  virtual void OnUpdate() override {}

  virtual void OnDeactivate() override {
  }

 private:
  EditorApp* m_App;
  std::filesystem::path m_CurrentDirectory;
  Hydrogen::ReferencePointer<Hydrogen::Texture2D> m_DirectoryIcon;
  Hydrogen::ReferencePointer<Hydrogen::Texture2D> m_FileIcon;
  Hydrogen::ReferencePointer<Panel> m_CurrentAssetPanel;
};
}  // namespace HydrogenEditor
