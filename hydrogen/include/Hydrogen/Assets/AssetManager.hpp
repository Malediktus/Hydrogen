#pragma once

#include <filesystem>
#include <unordered_map>

#include "../Core/Memory.hpp"
#include "MeshAsset.hpp"
#include "ShaderAsset.hpp"
#include "SpriteAsset.hpp"

namespace Hydrogen {
class AssetManager {
 public:
  static void Init();

  template <typename T>
  static ReferencePointer<T> Get(const std::filesystem::path& filename) {
    static_assert(std::is_base_of<class Asset, T>::value, "T must be derived from Asset");

    if (s_Assets.count(filename.string())) return std::dynamic_pointer_cast<T>(s_Assets[filename.string()]);

    std::filesystem::path filepath(filename);
    if (!(std::filesystem::exists(filepath))) {
      return std::dynamic_pointer_cast<T>(s_Assets[filename.string()]);
    }

    auto extension = filepath.extension().string();
    if (SpriteAsset::CheckFileExtensions(extension)) {
      auto ref = NewReferencePointer<SpriteAsset>();
      ref->Load(filename);
      s_Assets[filename.string()] = ref;
    } else if (ShaderAsset::CheckFileExtensions(extension)) {
      auto ref = NewReferencePointer<ShaderAsset>();
      ref->Load(filename);
      s_Assets[filename.string()] = ref;
    } else if (MeshAsset::CheckFileExtensions(extension)) {
      auto ref = NewReferencePointer<MeshAsset>();
      ref->Load(filename);
      s_Assets[filename.string()] = ref;
    }

    return std::dynamic_pointer_cast<T>(s_Assets[filename.string()]);
  }

 private:
  static std::unordered_map<String, ReferencePointer<Asset>> s_Assets;
};
}  // namespace Hydrogen
