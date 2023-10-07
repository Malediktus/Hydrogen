#include <Hydrogen/Assets/AssetManager.hpp>
#include <Hydrogen/Core/Logger.hpp>

using namespace Hydrogen;

std::unordered_map<std::filesystem::path, ReferencePointer<Asset>> AssetManager::s_Assets;

void AssetManager::Init() {
  for (const auto& dirEntry : std::filesystem::recursive_directory_iterator("assets")) {
    if (dirEntry.is_directory() && dirEntry.path().extension().string() != ".glsl") continue;
    if (dirEntry.is_symlink())  // TODO: Maybe use symlinks too
      continue;

    HY_LOG_DEBUG("Asset file found: {}", dirEntry.path().string());

    auto filename = dirEntry.path();
    auto extension = filename.extension().string();
    if (SpriteAsset::CheckFileExtensions(extension)) {
      auto ref = NewReferencePointer<SpriteAsset>();
      if (ref->GetInfo().Preload) ref->Load(filename);
      s_Assets[filename] = ref;
    } else if (ShaderAsset::CheckFileExtensions(extension)) {
      auto ref = NewReferencePointer<ShaderAsset>();
      ref->Load(filename);
      s_Assets[filename] = ref;
    } else if (MeshAsset::CheckFileExtensions(extension)) {
      auto ref = NewReferencePointer<MeshAsset>();
      if (ref->GetInfo().Preload) ref->Load(filename);
      s_Assets[filename] = ref;
    }
  }
}
