#pragma once

#include <filesystem>
#include "../Core/Memory.hpp"
#include "ShaderAsset.hpp"
#include "SpriteAsset.hpp"

namespace Hydrogen {
class AssetManager {
 public:
  static void Init();

  template <typename T>
  static ReferencePointer<T> Get(const String& filename) {
    static_assert(std::is_base_of<class Asset, T>::value, "T must be derived from Asset");

    if (s_Assets.count(filename)) return std::dynamic_pointer_cast<T>(s_Assets[filename]);

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
}  // namespace Hydrogen
