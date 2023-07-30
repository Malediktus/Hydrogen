#pragma once

#include <stb_image.h>

#include "../Renderer/Texture.hpp"
#include "Asset.hpp"

namespace Hydrogen {
class SpriteAsset : public Asset {
 public:
  SpriteAsset() { m_AssetInfo.Preload = true; }

  void Load(const String& filepath) override {
    HY_ASSERT(!filepath.empty(),
              "Parameter 'filepath' of type 'const String&' in function "
              "SpriteAsset::Load(const String& filepath) is an empty string!");
    HY_LOG_INFO("Loading sprite asset '{}'!", filepath);
    HY_LOG_INFO("Finished loading sprite asset '{}'!", filepath);
  }

  const ReferencePointer<Texture2D>& GetTexture() {
    HY_ASSERT(m_Texture,
              "SpriteAsset::GetTexture() was about to return null! "
              "SpriteAsset::Load(const String& filename) shall be called first!");
    return m_Texture;
  }

  static const DynamicArray<const String> GetFileExtensions() { return DynamicArray<const String>{".png", ".jpg", ".bmp", ".tga", ".hdr"}; }

  static bool CheckFileExtensions(const String& ext) {
    auto exts = GetFileExtensions();
    return std::find(exts.begin(), exts.end(), ext.c_str()) != exts.end();
  }

 private:
  ReferencePointer<Texture2D> m_Texture;
};
}  // namespace Hydrogen
