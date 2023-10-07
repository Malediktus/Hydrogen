#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Core/Assert.hpp"
#include "../Core/Logger.hpp"
#include "../Renderer/Texture.hpp"
#include "../Renderer/Buffer.hpp"
#include "../Renderer/VertexArray.hpp"
#include "../Scene/Scene.hpp"
#include "../Scene/Entity.hpp"
#include "../Scene/Components.hpp"
#include "Asset.hpp"

namespace Hydrogen {
class MeshAsset : public Asset {
 public:
  MeshAsset() {
    m_AssetInfo.Preload = true;
    m_Scene = nullptr;
  }

  void Load(const std::filesystem::path& filepath) override {
    HY_ASSERT(!filepath.empty(),
              "Parameter 'filepath' of type 'const String&' in function "
              "MeshAsset::Load(const String& filepath) is an empty string!");
    HY_LOG_INFO("Loading mesh asset '{}'!", filepath.string());
    // TODO: Load asset here
    m_Filepath = filepath;
    HY_LOG_INFO("Finished loading mesh asset '{}'!", filepath.string());
  }

  void Spawn(const ReferencePointer<class RenderDevice>& renderDevice, const ScopePointer<Scene>& scene, const String& name) {
    //HY_ASSERT(m_Scene, "MeshAsset not yet loaded!");
    Assimp::Importer importer;
    m_Scene = importer.ReadFile(m_Filepath.string(), aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    HY_ASSERT((m_Scene && !(m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && m_Scene->mRootNode), "Failed to load mesh file {}", m_Filepath.string());
    HandleNode(renderDevice, m_Scene->mRootNode, name, scene, Entity());
    importer.FreeScene();
  }
   
  static const DynamicArray<String> GetFileExtensions() { return DynamicArray<String>{".obj"}; }

  static bool CheckFileExtensions(const String& ext) {
    auto exts = GetFileExtensions();
    return std::find(exts.begin(), exts.end(), ext.c_str()) != exts.end();
  }

 private:
  void HandleNode(const ReferencePointer<class RenderDevice>& renderDevice, aiNode* node, const String& name, const ScopePointer<Scene>& scene,
                  Entity parent) {
    Entity entity;

    if (parent.GetEntityHandle() != entt::null) {
      entity = parent.CreateChild(node->mName.C_Str());
    } else {
      entity = scene->CreateEntity(name);
    }

    auto& meshRenderer = entity.AddComponent<MeshRendererComponent>();

    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
      aiVector3D* vertices = m_Scene->mMeshes[node->mMeshes[i]]->mVertices;
      aiVector3D* normals = m_Scene->mMeshes[node->mMeshes[i]]->mNormals;
      aiVector3D** texCoords = m_Scene->mMeshes[node->mMeshes[i]]->mTextureCoords;

      DynamicArray<float> vertexData;
      for (uint32_t j = 0; j < m_Scene->mMeshes[node->mMeshes[i]]->mNumVertices; j++) {
        vertexData.push_back(vertices[j].x);
        vertexData.push_back(vertices[j].y);
        vertexData.push_back(vertices[j].z);

        vertexData.push_back(normals[j].x);
        vertexData.push_back(normals[j].y);
        vertexData.push_back(normals[j].z);

        vertexData.push_back(texCoords[0][j].x);
        vertexData.push_back(texCoords[0][j].y);
      }

      DynamicArray<uint32_t> indexData;
      for (uint32_t j = 0; j < m_Scene->mMeshes[node->mMeshes[i]]->mNumFaces; j++) {
        aiFace face = m_Scene->mMeshes[node->mMeshes[i]]->mFaces[j];
        for (uint32_t k = 0; k < face.mNumIndices; k++) {
          indexData.push_back(face.mIndices[k]);
        }
      }

      auto vertexBuffer = VertexBuffer::Create(renderDevice, vertexData.data(), vertexData.size() * sizeof(float));
      vertexBuffer->SetLayout({{ShaderDataType::Float3, "Position", false}, {ShaderDataType::Float3, "Normal", false}, {ShaderDataType::Float2, "TexCoords", false}});
      auto indexBuffer = IndexBuffer::Create(renderDevice, indexData.data(), indexData.size() * sizeof(uint32_t));
      auto vertexArray = VertexArray::Create();
      vertexArray->AddVertexBuffer(vertexBuffer);
      vertexArray->SetIndexBuffer(indexBuffer);

      meshRenderer.VertexArrays.push_back(vertexArray);
    }

    if (node->mNumMeshes <= 0) {
      entity.RemoveComponent<MeshRendererComponent>();
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
      HandleNode(renderDevice, node->mChildren[i], name, scene, entity);
    }
  }

  std::filesystem::path m_Filepath;
  const aiScene* m_Scene;
};
}  // namespace Hydrogen
