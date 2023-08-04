#include <Hydrogen/Assets/AssetManager.hpp>

using namespace Hydrogen;

std::unordered_map<String, ReferencePointer<Asset>> AssetManager::s_Assets;

void AssetManager::Init() {
  for (const auto& dirEntry : std::filesystem::recursive_directory_iterator("assets")) {
    HY_LOG_DEBUG("Asset file found: {}", dirEntry.path().string());
    if (dirEntry.is_directory() && dirEntry.path().extension().string() != ".glsl") continue;

    if (dirEntry.is_symlink())  // TODO: Maybe use symlinks too
      continue;

    auto filename = dirEntry.path();
    auto extension = filename.extension().string();
    auto filenameString = filename.string();
    if (SpriteAsset::CheckFileExtensions(extension)) {
      auto ref = NewReferencePointer<SpriteAsset>();
      if (ref->GetInfo().Preload) ref->Load(filenameString);
      s_Assets[filenameString] = ref;
    } else if (ShaderAsset::CheckFileExtensions(extension)) {
      auto ref = NewReferencePointer<ShaderAsset>();
      if (ref->GetInfo().Preload) ref->Load(filenameString);
      s_Assets[filenameString] = ref;
    }
  }
}
