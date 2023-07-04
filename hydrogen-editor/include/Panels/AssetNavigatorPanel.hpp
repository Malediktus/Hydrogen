#pragma once

#include "../Panel.hpp"

namespace HydrogenEditor {
class AssetNavigatorPanel : public Panel {
 public:
  AssetNavigatorPanel() = default;
  ~AssetNavigatorPanel() = default;

  virtual void OnImGuiRender() override {
    if (m_CurrentDirectory != std::filesystem::path("assets")) {
      if (ImGui::Button("<-")) {
        m_CurrentDirectory = m_CurrentDirectory.parent_path();
      }
    }

    // for (auto& directoryEntry :
    // std::filesystem::directory_iterator(m_CurrentDirectory)) {
    //     const auto& path = directoryEntry.path();
    //     auto relativePath = std::filesystem::relative(path, "assets");
    //     std::string filenameString = relativePath.filename().string();
    //     if (directoryEntry.is_directory()) {
    //         if (ImGui::Button(filenameString.c_str())) {
    //             m_CurrentDirectory /= path.filename();
    //         }
    //     } else if (directoryEntry.is_regular_file()) {
    //         if (ImGui::Button(filenameString.c_str())) {
    //             // TODO: Open asset file panel
    //         }
    //     }
    // }

    static float padding = 16.0f;
    static float thumbnailSize = 128.0f;
    float cellSize = thumbnailSize + padding;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1) columnCount = 1;

    // ImGui::Columns(columnCount, 0, false);

    for (auto& directoryEntry :
         std::filesystem::directory_iterator(m_CurrentDirectory)) {
      const auto& path = directoryEntry.path();
      auto relativePath = std::filesystem::relative(path, "assets");
      std::string filenameString = relativePath.filename().string();

      Hydrogen::ReferencePointer<Hydrogen::Texture2D> icon =
          directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
      // ImGui::ImageButton(*(ImTextureID*) icon->GetNative(), {thumbnailSize,
      // thumbnailSize}, {0, 1}, {1, 0}); if (ImGui::IsItemHovered() &&
      // //ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
      //    if (directoryEntry.is_directory())
      //        m_CurrentDirectory /= path.filename();
      //}
      // ImGui::TextWrapped("%s", filenameString.c_str());

      // ImGui::NextColumn();
    }

    // ImGui::Columns(1);

    // ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
    // ImGui::SliderFloat("Padding", &padding, 0, 32);
    ImGui::Image(*(ImTextureID*)m_DirectoryIcon->GetNative(),
                 {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});
  }

  virtual const Hydrogen::String GetTitle() override {
    return "Asset Navigator";
  }

  virtual void OnActivate() override {
    m_CurrentDirectory = "assets";  // TODO: Get from asset manager
    m_DirectoryIcon = Hydrogen::AssetManager::Get<Hydrogen::SpriteAsset>(
                          "assets/Textures/DirectoryIcon.png")
                          ->GetTexture();
    m_FileIcon = Hydrogen::AssetManager::Get<Hydrogen::SpriteAsset>(
                     "assets/Textures/FileIcon.png")
                     ->GetTexture();
  }

  virtual void OnUpdate() override {}

  virtual void OnDeactivate() override {}

 private:
  std::filesystem::path m_CurrentDirectory;
  Hydrogen::ReferencePointer<Hydrogen::Texture2D> m_DirectoryIcon;
  Hydrogen::ReferencePointer<Hydrogen::Texture2D> m_FileIcon;
};
}  // namespace HydrogenEditor
