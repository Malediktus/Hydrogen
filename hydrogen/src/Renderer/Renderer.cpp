#include <Hydrogen/Assets/AssetManager.hpp>
#include <Hydrogen/Renderer/Buffer.hpp>
#include <Hydrogen/Renderer/RenderDevice.hpp>
#include <Hydrogen/Renderer/RenderWindow.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/Shader.hpp>
#include <Hydrogen/Renderer/SurfaceAttachment.hpp>
#include <Hydrogen/Renderer/VertexArray.hpp>
#include <Hydrogen/Scene/Components.hpp>
#include <Hydrogen/Scene/Scene.hpp>
#include <tracy/Tracy.hpp>

#define GLM_FORCE_RADIANS
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Hydrogen;

struct UniformBufferObject {
  alignas(16) glm::mat4 View;
  alignas(16) glm::mat4 Proj;
};

struct ObjectInformation {
  alignas(16) glm::mat4 Model;
};

struct LightData {
  alignas(16) glm::vec3 LightPos;
  alignas(16) glm::vec3 ViewPos;
  alignas(16) glm::vec3 LightColor;
  alignas(16) glm::vec3 ObjectColor;
};

ReferencePointer<Context> Renderer::s_Context;
ReferencePointer<RenderDevice> Renderer::s_RenderDevice;

Renderer::Renderer(const ReferencePointer<RenderWindow>& window) : m_RenderWindow(window) {
  ZoneScoped;

  m_WhiteTexture = AssetManager::Get<SpriteAsset>("assets/Textures/WhiteTexture.png")->GetTexture2D();
  m_UniformBuffer = UniformBuffer::Create(sizeof(UniformBufferObject));
  m_ObjectInformation = UniformBuffer::Create(sizeof(ObjectInformation));
  m_LightBuffer = UniformBuffer::Create(sizeof(LightData));

  ShaderDependency uniformBuffer{};
  uniformBuffer.Type = ShaderDependencyType::UniformBuffer;
  uniformBuffer.Stage = ShaderStage::VertexShader;
  uniformBuffer.Location = 0;

  ShaderDependency objectInformation{};
  objectInformation.Type = ShaderDependencyType::DynamicUniformBuffer;
  objectInformation.Stage = ShaderStage::VertexShader;
  objectInformation.Location = 4;

  ShaderDependency lightBuffer{};
  lightBuffer.Type = ShaderDependencyType::UniformBuffer;
  lightBuffer.Stage = ShaderStage::PixelShader;
  lightBuffer.Location = 1;

  ShaderDependency diffuseTexture{};
  diffuseTexture.Type = ShaderDependencyType::Texture;
  diffuseTexture.Stage = ShaderStage::PixelShader;
  diffuseTexture.Location = 2;

  ShaderDependency specularTexture{};
  specularTexture.Type = ShaderDependencyType::Texture;
  specularTexture.Stage = ShaderStage::PixelShader;
  specularTexture.Location = 3;

  m_Shader =
      AssetManager::Get<ShaderAsset>("assets/Raw.glsl")
          ->GetShader(m_RenderWindow, {{ShaderDataType::Float3, "Position", false}, {ShaderDataType::Float3, "Normal", false}, {ShaderDataType::Float2, "TexCoords", false}},
                         {uniformBuffer, lightBuffer, diffuseTexture, specularTexture, objectInformation});

  LightData lightData{};
  lightData.LightPos = glm::vec3(1.2f, 1.0f, 2.0f);
  lightData.ViewPos = glm::vec3(0.0f, 0.0f, 0.0f);
  lightData.LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
  lightData.ObjectColor = glm::vec3(1.0f, 0.5f, 0.31f);

  m_LightBuffer->SetData(&lightData);
  m_Shader->SetBuffer(m_LightBuffer, 1);
  m_Shader->SetBuffer(m_UniformBuffer, 0);
  m_Shader->SetTexture(m_WhiteTexture, 2);
  m_Shader->SetTexture(m_WhiteTexture, 3);
  m_Shader->SetDynamicBuffer(m_ObjectInformation, 4);

  HY_LOG_INFO("Initialized renderer");
}

Renderer::~Renderer() { s_RenderDevice->WaitForIdle(); }

void Renderer::BeginFrame() {
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

  UniformBufferObject ubo{};
  ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  const auto& viewportSize = m_RenderWindow->GetViewportSize();
  ubo.Proj = glm::perspective(glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.001f, 1000.0f);
  ubo.Proj[1][1] *= -1;

  m_UniformBuffer->SetData(&ubo);

  RendererAPI::Get()->BeginFrame();
  RendererAPI::Get()->SetViewport();
  RendererAPI::Get()->SetScissor();
}

void Renderer::Submit(const ReferencePointer<VertexArray>& vertexArray, const ReferencePointer<Shader>& shader, const glm::mat4& transform) {
  ObjectInformation info{};
  info.Model = transform;

  m_ObjectInformation->SetData(&info);

  m_Shader->Bind();
  vertexArray->Bind();
  RendererAPI::Get()->DrawIndexed(vertexArray);
}

void Renderer::EndFrame() { RendererAPI::Get()->EndFrame(); }
