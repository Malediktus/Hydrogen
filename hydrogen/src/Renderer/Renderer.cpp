#include <Hydrogen/Assets/AssetManager.hpp>
#include <Hydrogen/Renderer/Buffer.hpp>
#include <Hydrogen/Renderer/CommandBuffer.hpp>
#include <Hydrogen/Renderer/Framebuffer.hpp>
#include <Hydrogen/Renderer/RenderDevice.hpp>
#include <Hydrogen/Renderer/RenderWindow.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/Shader.hpp>
#include <Hydrogen/Renderer/SwapChain.hpp>
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
  alignas(16) glm::mat4 Model;
  alignas(16) glm::mat4 View;
  alignas(16) glm::mat4 Proj;
};

struct LightData {
  alignas(16) glm::vec3 LightPos;
  alignas(16) glm::vec3 ViewPos;
  alignas(16) glm::vec3 LightColor;
  alignas(16) glm::vec3 ObjectColor;
};

ReferencePointer<Context> Renderer::s_Context;
ReferencePointer<RenderDevice> Renderer::s_RenderDevice;
uint32_t Renderer::s_MaxFramesInFlight = MAX_FRAMES_IN_FLIGHT;

Renderer::Renderer(const ReferencePointer<RenderWindow>& window, const ScopePointer<class Scene>& scene) : m_RenderWindow(window), m_Scene(scene) {
  ZoneScoped;

  m_SwapChain = SwapChain::Create(window, s_RenderDevice, true);
  m_Framebuffer = Framebuffer::Create(s_RenderDevice, m_SwapChain);
  m_WhiteTexture = AssetManager::Get<SpriteAsset>("assets/Textures/WhiteTexture.png")->CreateTexture2D(s_RenderDevice);
  m_UniformBuffer = UniformBuffer::Create(s_RenderDevice, sizeof(UniformBufferObject));

  m_LightBuffer = UniformBuffer::Create(s_RenderDevice, sizeof(LightData));

  ShaderDependency uniformBuffer{};
  uniformBuffer.Type = ShaderDependencyType::UniformBuffer;
  uniformBuffer.Stage = ShaderStage::VertexShader;
  uniformBuffer.Location = 0;

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

  m_Shader = AssetManager::Get<ShaderAsset>("assets/Raw.glsl")
                 ->CreateShader(s_RenderDevice, m_SwapChain, m_Framebuffer,
                                {{ShaderDataType::Float3, "Position", false}, {ShaderDataType::Float3, "Normal", false}, {ShaderDataType::Float2, "TexCoords", false}},
                                {uniformBuffer, lightBuffer, diffuseTexture, specularTexture});

  m_CommandBuffers.resize(s_MaxFramesInFlight);
  for (uint32_t i = 0; i < s_MaxFramesInFlight; i++) {
    m_CommandBuffers[i] = CommandBuffer::Create(s_RenderDevice);
  }

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

  m_CurrentFrame = 0;

  HY_LOG_INFO("Initialized renderer");
}

Renderer::~Renderer() { s_RenderDevice->WaitForIdle(); }

void Renderer::Render() {
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

  UniformBufferObject ubo{};
  ubo.Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.Model = glm::scale(ubo.Model, glm::vec3(0.2f, 0.2f, 0.2f));
  ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  const auto& viewportSize = m_RenderWindow->GetViewportSize();
  ubo.Proj = glm::perspective(glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.001f, 1000.0f);
  ubo.Proj[1][1] *= -1;

  m_UniformBuffer->SetData(&ubo);

  auto entities = m_Scene->GetEntities();

  const auto& commandBuffer = m_CommandBuffers[m_CurrentFrame];

  commandBuffer->Reset();
  m_SwapChain->AcquireNextImage(commandBuffer);

  commandBuffer->Begin();
  m_Framebuffer->Begin({0.0f, 0.0f, 0.0f, 1.0f}, commandBuffer);
  m_Shader->Bind(commandBuffer);

  commandBuffer->CmdSetViewport(m_SwapChain);
  commandBuffer->CmdSetScissor(m_SwapChain);

  for (auto& entity : entities) {
    if (entity.HasComponent<MeshRendererComponent>()) {
      for (auto& mesh : entity.GetComponent<MeshRendererComponent>().Meshes) {
        auto& material = entity.GetComponent<MeshRendererComponent>()._Material;
        if (material.DiffuseMaps.size() > 0) {
          // m_Shader->SetTexture(material.DiffuseMaps[0], 2);
        } else {
          // m_Shader->SetTexture(m_WhiteTexture, 2);
        }

        if (material.SpecularMaps.size() > 0) {
          // m_Shader->SetTexture(material.SpecularMaps[0], 3);
        } else {
          // m_Shader->SetTexture(m_WhiteTexture, 3);
        }

        RenderMesh(commandBuffer, mesh.VertexArray);
      }
    }

    auto children = entity.GetChildren();
    for (auto& child : children) {
      if (child.HasComponent<MeshRendererComponent>()) {
        for (auto& mesh : child.GetComponent<MeshRendererComponent>().Meshes) {
          auto& material = child.GetComponent<MeshRendererComponent>()._Material;
          if (material.DiffuseMaps.size() > 0) {
            // m_Shader->SetTexture(material.DiffuseMaps[0], 2);
          } else {
            // m_Shader->SetTexture(m_WhiteTexture, 2);
          }

          if (material.SpecularMaps.size() > 0) {
            // m_Shader->SetTexture(material.SpecularMaps[0], 3);
          } else {
            // m_Shader->SetTexture(m_WhiteTexture, 3);
          }

          RenderMesh(commandBuffer, mesh.VertexArray);
        }
      }
    }
  }

  commandBuffer->CmdDrawImGuiDrawData();
  m_Framebuffer->End(commandBuffer);
  commandBuffer->End();

  commandBuffer->CmdUploadResources();
  commandBuffer->CmdDisplayImage(m_SwapChain);

  m_CurrentFrame = (m_CurrentFrame + 1) % s_MaxFramesInFlight;
}

void Renderer::RenderMesh(const ReferencePointer<CommandBuffer>& commandBuffer, const ReferencePointer<class VertexArray>& vertexArray) {
  vertexArray->Bind(commandBuffer);
  commandBuffer->CmdDrawIndexed(vertexArray);
}
