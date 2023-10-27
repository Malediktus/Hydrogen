#include <Hydrogen/Assets/MeshAsset.hpp>
#include <Hydrogen/Assets/AssetManager.hpp>
#include <Hydrogen/Assets/SpriteAsset.hpp>
#include <Hydrogen/Renderer/Texture.hpp>
#include <Hydrogen/Renderer/Buffer.hpp>
#include <Hydrogen/Renderer/VertexArray.hpp>
#include <Hydrogen/Scene/Scene.hpp>
#include <Hydrogen/Scene/Entity.hpp>

using namespace Hydrogen;

void MeshAsset::Load(const std::filesystem::path& filepath) {
  HY_ASSERT(!filepath.empty(),
            "Parameter 'filepath' of type 'const String&' in function "
            "MeshAsset::Load(const String& filepath) is an empty string!");
  HY_LOG_INFO("Loading mesh asset '{}'!", filepath.string());
  // TODO: Load asset here
  m_Filepath = filepath;
  HY_LOG_INFO("Finished loading mesh asset '{}'!", filepath.string());
}

void MeshAsset::Spawn(const ReferencePointer<class RenderDevice>& renderDevice, const ScopePointer<Scene>& scene, const String& name) {
  // HY_ASSERT(m_Scene, "MeshAsset not yet loaded!");
  Assimp::Importer importer;
  m_Scene = importer.ReadFile(m_Filepath.string(), aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
  HY_ASSERT((m_Scene && !(m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && m_Scene->mRootNode), "Failed to load mesh file {}", m_Filepath.string());
  HandleNode(renderDevice, m_Scene->mRootNode, name, scene, Entity());
  importer.FreeScene();
}

void MeshAsset::HandleNode(const ReferencePointer<class RenderDevice>& renderDevice, aiNode* node, const String& name, const ScopePointer<Scene>& scene, Entity parent) {
  Entity entity;
  // TODO: Set transform

  if (parent.GetEntityHandle() != entt::null) {
    entity = parent.CreateChild(node->mName.C_Str());
  } else {
    entity = scene->CreateEntity(name);
  }

  auto& meshRenderer = entity.AddComponent<MeshRendererComponent>();

  for (uint32_t i = 0; i < node->mNumMeshes; i++) {
    aiMesh* currentMesh = m_Scene->mMeshes[node->mMeshes[i]];

    MeshRendererComponent::Mesh mesh;
    mesh.VertexArray = CreateVertexArrayForMesh(renderDevice, currentMesh);

    meshRenderer.Meshes.push_back(mesh);

    meshRenderer._Material.Shininess = 30.0f;
    if (currentMesh->mMaterialIndex >= 0) {
      aiMaterial* currentMaterial = m_Scene->mMaterials[currentMesh->mMaterialIndex];
      meshRenderer._Material = LoadMaterial(renderDevice, currentMaterial);
    }
  }

  if (node->mNumMeshes <= 0) {
    entity.RemoveComponent<MeshRendererComponent>();
  }

  for (uint32_t i = 0; i < node->mNumChildren; i++) {
    HandleNode(renderDevice, node->mChildren[i], name, scene, entity);
  }
}

ReferencePointer<VertexArray> MeshAsset::CreateVertexArrayForMesh(const ReferencePointer<RenderDevice>& renderDevice, aiMesh* mesh) {
  aiVector3D* vertices = mesh->mVertices;
  aiVector3D* normals = mesh->mNormals;
  aiVector3D** texCoords = mesh->mTextureCoords;

  DynamicArray<float> vertexData;
  for (uint32_t j = 0; j < mesh->mNumVertices; j++) {
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
  for (uint32_t j = 0; j < mesh->mNumFaces; j++) {
    aiFace face = mesh->mFaces[j];
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

  return vertexArray;
}

MeshRendererComponent::Material MeshAsset::LoadMaterial(const ReferencePointer<RenderDevice>& renderDevice, aiMaterial* material) {
  MeshRendererComponent::Material result;

  for (uint32_t i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
    aiString str;
    material->GetTexture(aiTextureType_DIFFUSE, i, &str);
    result.DiffuseMaps.push_back(AssetManager::Get<SpriteAsset>(str.C_Str())->CreateTexture2D(renderDevice));
  }

  for (uint32_t i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); i++) {
    aiString str;
    material->GetTexture(aiTextureType_SPECULAR, i, &str);
    result.SpecularMaps.push_back(AssetManager::Get<SpriteAsset>(str.C_Str())->CreateTexture2D(renderDevice));
  }

  return result;
}
