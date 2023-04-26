#pragma once

#include "../Core/Memory.hpp"
#include "../Core/Logger.hpp"

#include <filesystem>

namespace Hydrogen {
class Asset {
public:
    struct AssetInfo {
        bool Preload;
    };

    virtual ~Asset() = default;
    virtual void Load(const std::string& filepath) = 0;

    AssetInfo GetInfo() {
        return m_AssetInfo;
    }

protected:
    AssetInfo m_AssetInfo;
};

class SpriteAsset : public Asset {
public:
    SpriteAsset() {
        m_AssetInfo.Preload = true;
    }

    void Load(const std::string& filepath) override {
        HY_LOG_DEBUG("Loading sprite: {}", filepath);
    }

    static const std::vector<const std::string> GetFileExtensions() {
        return std::vector<const std::string> {".png", ".jpg", ".bmp", ".tga", ".hdr"};
    }

    static bool CheckFileExtensions(const std::string& ext) {
        auto exts = GetFileExtensions();
        return std::find(exts.begin(), exts.end(), ext.c_str()) != exts.end();
    }
};

class AssetManager {
public:
    static void Init() {
        HY_LOG_DEBUG("Current working directory: {}", std::filesystem::current_path());

        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator("assets")) {
            HY_LOG_DEBUG("Asset file found: {}", dirEntry);
            if (dirEntry.is_directory() || dirEntry.is_symlink()) // TODO: Maybe use symlinks too
                continue;

            auto filename = dirEntry.path();
            auto extension = filename.extension().string();
            auto filenameString = filename.string();
            if (SpriteAsset::CheckFileExtensions(extension)) {
                auto ref = NewReference<SpriteAsset>();
                if (ref->GetInfo().Preload)
                    ref->Load(filenameString);
                s_Assets[filenameString] = ref;
            }
        }
    }

    static Reference<Asset> Get(const std::string& filename) {
        if (s_Assets[filename])
            return s_Assets[filename];

        std::filesystem::path filepath(filename);
        if (!(std::filesystem::exists(filepath))) {
            return s_Assets[filename];
        }

        auto extension = filepath.extension().string();
        auto filenameString = filepath.string();
        if (extension == ".png" || extension == ".jpg" || extension == ".bmp" || extension == ".tga" || extension == ".hdr") {
            auto ref = NewReference<SpriteAsset>();
            if (ref->GetInfo().Preload)
                ref->Load(filenameString);
            s_Assets[filenameString] = ref;
        }

        return s_Assets[filenameString];
    }

private:
    static std::unordered_map<std::string, Reference<Asset>> s_Assets;
};
} // namespace Hydrogen
