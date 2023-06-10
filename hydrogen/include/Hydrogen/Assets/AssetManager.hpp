#pragma once

#include "../Core/Memory.hpp"
#include "../Core/Logger.hpp"
#include "../Renderer/Shader.hpp"
#include "../Renderer/Texture.hpp"
#include <stb_image.h>

#include <filesystem>

namespace Hydrogen {
class Asset {
public:
    struct AssetInfo {
        bool Preload;
    };

    virtual ~Asset() = default;
    virtual void Load(const String& filepath) = 0;

    AssetInfo GetInfo() {
        return m_AssetInfo;
    }

protected:
    AssetInfo m_AssetInfo;
};

class UnknownAsset : public Asset {
public:
    UnknownAsset() {
        m_AssetInfo.Preload = false;
    }

    void Load(const String&) override {
        HY_LOG_WARN("Trying to load asset of type unknown");
    }
};

class SpriteAsset : public Asset {
public:
    SpriteAsset() {
        m_AssetInfo.Preload = true;
    }

    // TODO: This code entire loading code is temporary
    void Load(const String& filepath) override {
        HY_LOG_DEBUG("Loading sprite: {}", filepath);

        int width, height, nrChannels;
        uint8_t* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

        Texture2DStorageType storageType = Texture2DStorageType::RGBA8F;
        if (nrChannels == 3) {
            storageType = Texture2DStorageType::RGB8F;
        }

        m_Texture = Texture2D::Create(width, height, data, storageType);
        stbi_image_free(data);
    }

    const ReferencePointer<Texture2D>& GetTexture() {
        return m_Texture;
    }

    static const DynamicArray<const String> GetFileExtensions() {
        return DynamicArray<const String> {".png", ".jpg", ".bmp", ".tga", ".hdr"};
    }

    static bool CheckFileExtensions(const String& ext) {
        auto exts = GetFileExtensions();
        return std::find(exts.begin(), exts.end(), ext.c_str()) != exts.end();
    }

private:
    ReferencePointer<Texture2D> m_Texture;
};

class ShaderAsset : public Asset {
public:
    ShaderAsset() {
        m_AssetInfo.Preload = true;
    }

    void Load(const String& filepath) override {
        HY_LOG_DEBUG("Loading shader: {}", filepath);
        m_Shader = Shader::Create(filepath);
    }

    const ReferencePointer<Shader>& GetShader() {
        return m_Shader;
    }

    static const DynamicArray<const String> GetFileExtensions() {
        return DynamicArray<const String> {".glsl"};
    }

    static bool CheckFileExtensions(const String& ext) {
        auto exts = GetFileExtensions();
        return std::find(exts.begin(), exts.end(), ext.c_str()) != exts.end();
    }

private:
    ReferencePointer<Shader> m_Shader;
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
                auto ref = NewReferencePointer<SpriteAsset>();
                if (ref->GetInfo().Preload)
                    ref->Load(filenameString);
                s_Assets[filenameString] = ref;
            } else if (ShaderAsset::CheckFileExtensions(extension)) {
                auto ref = NewReferencePointer<ShaderAsset>();
                if (ref->GetInfo().Preload)
                    ref->Load(filenameString);
                s_Assets[filenameString] = ref;
            } else {
                auto ref = NewReferencePointer<UnknownAsset>();
                s_Assets[filenameString] = ref;
            }
        }
    }

    template <typename T> static ReferencePointer<T> Get(const String& filename) {
        static_assert(std::is_base_of<Asset, T>::value, "T must be derived from Asset");

        if (s_Assets[filename])
            return std::dynamic_pointer_cast<T>(s_Assets[filename]);

        std::filesystem::path filepath(filename);
        if (!(std::filesystem::exists(filepath))) {
            return std::dynamic_pointer_cast<T>(s_Assets[filename]);
        }

        auto extension = filepath.extension().string();
        auto filenameString = filepath.string();
        if (SpriteAsset::CheckFileExtensions(extension)) {
            auto ref = NewReferencePointer<SpriteAsset>();
            ref->Load(filenameString);
            s_Assets[filenameString] = ref;
        } else if (ShaderAsset::CheckFileExtensions(extension)) {
            auto ref = NewReferencePointer<ShaderAsset>();
            ref->Load(filenameString);
            s_Assets[filenameString] = ref;
        }

        return std::dynamic_pointer_cast<T>(s_Assets[filenameString]);
    }

private:
    static std::unordered_map<String, ReferencePointer<Asset>> s_Assets;
};
} // namespace Hydrogen
