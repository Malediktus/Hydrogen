#pragma once

#include <stb_image.h>

#include "../Core/Assert.hpp"
#include "../Core/Logger.hpp"
#include "../Core/Memory.hpp"
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

  void Load(const std::filesystem::path& filepath) override;
  const ReferencePointer<class Texture2D>& GetTexture2D() { return m_Texture; }

  static const DynamicArray<String> GetFileExtensions() { return DynamicArray<String>{".jpg", ".jpeg", ".png", ".tga", ".bmp", ".psd", ".gif", ".hdr", ".pic", ".pnm"}; }

  static bool CheckFileExtensions(const String& ext) {
    auto exts = GetFileExtensions();
    return std::find(exts.begin(), exts.end(), ext.c_str()) != exts.end();
  }

 private:
  ReferencePointer<class Texture2D> m_Texture;

  stbi_uc* m_Pixels;
  uint32_t m_Channels;
  uint32_t m_Width;
  uint32_t m_Height;
};
}  // namespace Hydrogen
