#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Core/Assert.hpp"
#include "../Core/Logger.hpp"
#include "../Core/Memory.hpp"
#include "../Scene/Components.hpp"
#include "Asset.hpp"

namespace Hydrogen {
class MeshAsset : public Asset {
 public:
  MeshAsset() {
    m_AssetInfo.Preload = true;
    m_Scene = nullptr;
  }

  void Load(const std::filesystem::path& filepath) override;
  void Spawn(const ReferencePointer<class RenderDevice>& renderDevice, const ScopePointer<class Scene>& scene, const String& name);

  static const DynamicArray<String> GetFileExtensions() { return DynamicArray<String>{".obj", ".fbx"}; }

  static bool CheckFileExtensions(const String& ext) {
    auto exts = GetFileExtensions();
    return std::find(exts.begin(), exts.end(), ext.c_str()) != exts.end();
  }

 private:
  void HandleNode(const ReferencePointer<class RenderDevice>& renderDevice, aiNode* node, const String& name, const ScopePointer<Scene>& scene, class Entity parent);
  ReferencePointer<class VertexArray> CreateVertexArrayForMesh(const ReferencePointer<RenderDevice>& renderDevice, aiMesh* mesh);
  MeshRendererComponent::Material LoadMaterial(const ReferencePointer<RenderDevice>& renderDevice, aiMaterial* material);

  std::filesystem::path m_Filepath;
  const aiScene* m_Scene;
};
}  // namespace Hydrogen
