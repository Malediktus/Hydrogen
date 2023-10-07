#pragma once

#include <stb_image.h>

#include "../Renderer/Texture.hpp"
#include "../Core/Assert.hpp"
#include "../Core/Logger.hpp"
#include "Asset.hpp"

namespace Hydrogen {
class SpriteAsset : public Asset {
 public:
  SpriteAsset() {
    m_AssetInfo.Preload = true;
    m_Pixels = nullptr;
    m_Channels = 0;
    m_Width = 0;
    m_Height = 0;
  }

  void Load(const std::filesystem::path& filepath) override {
    HY_ASSERT(!filepath.empty(),
              "Parameter 'filepath' of type 'const String&' in function "
              "SpriteAsset::Load(const String& filepath) is an empty string!");
    HY_LOG_INFO("Loading sprite asset '{}'!", filepath.string());
    m_Pixels = stbi_load(filepath.string().c_str(), (int*)&m_Width, (int*)&m_Height, (int*)&m_Channels, STBI_rgb_alpha);
    HY_ASSERT(m_Pixels, "Failed to load sprite {}", filepath.string());
    HY_LOG_INFO("Finished loading sprite asset '{}'!", filepath.string());
  }

  ReferencePointer<Texture2D> CreateTexture2D(const ReferencePointer<RenderDevice>& renderDevice) {
    HY_ASSERT(m_Pixels, "SpriteAsset already created Texture2D or is uninitialized!");
    auto texture = Texture2D::Create(renderDevice, m_Width, m_Height, m_Pixels);
    stbi_image_free(m_Pixels);
    m_Pixels = nullptr;
    return texture;
  }

  static const DynamicArray<String> GetFileExtensions() { return DynamicArray<String>{".jpg", ".jpeg", ".png", ".tga", ".bmp", ".psd", ".gif", ".hdr", ".pic", ".pnm"}; }

  static bool CheckFileExtensions(const String& ext) {
    auto exts = GetFileExtensions();
    return std::find(exts.begin(), exts.end(), ext.c_str()) != exts.end();
  }

 private:
  stbi_uc* m_Pixels;
  uint32_t m_Channels;
  uint32_t m_Width;
  uint32_t m_Height;
};
}  // namespace Hydrogen
