#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "../Core/Assert.hpp"
#include "../Core/Logger.hpp"
#include "../Core/Memory.hpp"
#include "../Scene/Components.hpp"
#include "Asset.hpp"

namespace Hydrogen {
struct MeshNode {
  DynamicArray<ReferencePointer<MeshNode>> Chilren;
  DynamicArray<MeshRendererComponent::Mesh> Meshes;
  MeshRendererComponent::Material Material;
  glm::mat4 Transform;
};

class MeshAsset : public Asset {
 public:
  MeshAsset() {
    m_AssetInfo.Preload = true;
    m_Scene = nullptr;
  }

  void Load(const std::filesystem::path& filepath) override;
  const ReferencePointer<MeshNode>& GetTree() { return m_Tree; }

  static const DynamicArray<String> GetFileExtensions() { return DynamicArray<String>{".obj", ".fbx"}; }

  static bool CheckFileExtensions(const String& ext) {
    auto exts = GetFileExtensions();
    return std::find(exts.begin(), exts.end(), ext.c_str()) != exts.end();
  }

 private:
  ReferencePointer<MeshNode> HandleNode(aiNode* node);
  ReferencePointer<class VertexArray> CreateVertexArrayForMesh(aiMesh* mesh);
  MeshRendererComponent::Material LoadMaterial(aiMaterial* material);

  ReferencePointer<MeshNode> m_Tree;
  std::filesystem::path m_Filepath;
  const aiScene* m_Scene;
};
}  // namespace Hydrogen
