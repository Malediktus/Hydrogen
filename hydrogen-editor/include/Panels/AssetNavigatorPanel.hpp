#pragma once

#include "../Panel.hpp"

namespace HydrogenEditor {
class AssetNavigatorPanel : public Panel {
public:
    AssetNavigatorPanel() = default;
    ~AssetNavigatorPanel() = default;

    virtual void OnImGuiRender() override {
        ImGui::Text("Assets");

        if (m_CurrentDirectory != std::filesystem::path("assets")) {
            if (ImGui::Button("<-")) {
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
        }

        for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
            const auto& path = directoryEntry.path();
            auto relativePath = std::filesystem::relative(path, "assets");
            std::string filenameString = relativePath.filename().string();
            if (directoryEntry.is_directory()) {
                if (ImGui::Button(filenameString.c_str())) {
                    m_CurrentDirectory /= path.filename();
                }
            } else if (directoryEntry.is_regular_file()) {
                if (ImGui::Button(filenameString.c_str())) {
                    // TODO: Open asset file panel
                }
            }
        }
    }

    virtual const Hydrogen::String GetTitle() override {
        return "Asset Navigator";
    }

    virtual void OnActivate() override {
        m_CurrentDirectory = "assets"; // TODO: Get from asset manager
        m_DirectoryIcon = Hydrogen::AssetManager::Get<Hydrogen::SpriteAsset>("assets/Textures/DirectoryIcon.png")->GetTexture();
        m_FileIcon = Hydrogen::AssetManager::Get<Hydrogen::SpriteAsset>("assets/Textures/FileIcon.png")->GetTexture();
    }

    virtual void OnUpdate() override {
    }

    virtual void OnDeactivate() override {
    }

private:
    std::filesystem::path m_CurrentDirectory;
    Hydrogen::ReferencePointer<Hydrogen::Texture2D> m_DirectoryIcon;
    Hydrogen::ReferencePointer<Hydrogen::Texture2D> m_FileIcon;
};
} // namespace HydrogenEditor
