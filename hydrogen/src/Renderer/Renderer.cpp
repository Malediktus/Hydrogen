#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/Buffer.hpp>
#include <Hydrogen/Renderer/SwapChain.hpp>
#include <Hydrogen/Renderer/Framebuffer.hpp>
#include <Hydrogen/Renderer/CommandBuffer.hpp>
#include <Hydrogen/Renderer/VertexArray.hpp>
#include <Hydrogen/Renderer/RenderWindow.hpp>
#include <Hydrogen/Assets/AssetManager.hpp>
#include <Hydrogen/Scene/Scene.hpp>
#include <Hydrogen/Scene/Components.hpp>
#include <tracy/Tracy.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

using namespace Hydrogen;

struct UniformBufferObject {
  alignas(16) glm::mat4 Model;
  alignas(16) glm::mat4 View;
  alignas(16) glm::mat4 Proj;
};

ReferencePointer<Context> Renderer::s_Context;
uint32_t Renderer::s_MaxFramesInFlight = MAX_FRAMES_IN_FLIGHT;

Renderer::Renderer(const ReferencePointer<RenderWindow>& window, const ReferencePointer<RenderDevice>& device, const ScopePointer<class Scene>& scene)
    : m_Device(device), m_RenderWindow(window), m_Scene(scene) {
  ZoneScoped;

  m_SwapChain = SwapChain::Create(window, device, true);
  m_Framebuffer = Framebuffer::Create(device, m_SwapChain);
  m_Texture = AssetManager::Get<SpriteAsset>("assets/Meshes/viking_room.png")->CreateTexture2D(m_Device);
  m_UniformBuffer = UniformBuffer::Create(m_Device, sizeof(UniformBufferObject));

  ShaderDependency uniformBuffer{};
  uniformBuffer.Type = ShaderDependencyType::UniformBuffer;
  uniformBuffer.Stage = ShaderStage::VertexShader;
  uniformBuffer.Location = 0;
  uniformBuffer.UniformBuffer = m_UniformBuffer;

  ShaderDependency texture{};
  texture.Type = ShaderDependencyType::Texture;
  texture.Stage = ShaderStage::PixelShader;
  texture.Location = 1;
  texture.Texture = m_Texture;

  m_Shader = AssetManager::Get<ShaderAsset>("assets/Raw.glsl")
                 ->CreateShader(device, m_SwapChain, m_Framebuffer,
                         {{ShaderDataType::Float3, "Position", false}, {ShaderDataType::Float3, "Normal", false}, {ShaderDataType::Float2, "TexCoords", false}}, {uniformBuffer, texture});

  m_CommandBuffers.resize(s_MaxFramesInFlight);
  for (uint32_t i = 0; i < s_MaxFramesInFlight; i++) {
    m_CommandBuffers[i] = CommandBuffer::Create(device);
  }

  m_CurrentFrame = 0;

  HY_LOG_INFO("Initialized renderer");
}

Renderer::~Renderer() { m_Device->WaitForIdle(); }

void Renderer::Render() {
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

  UniformBufferObject ubo{};
  ubo.Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  const auto& viewportSize = m_RenderWindow->GetViewportSize();
  ubo.Proj = glm::perspective(glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.001f, 1000.0f);
  ubo.Proj[1][1] *= -1;

  m_UniformBuffer->SetData(&ubo);
  const auto& commandBuffer = m_CommandBuffers[m_CurrentFrame];

  auto entities = m_Scene->GetEntitiesByName("Room");
  auto children = entities[0].GetChildrenByName("mesh_all1_Texture1_0");
  auto& entity = children[0];
  auto& vertexArray = entity.GetComponent<MeshRendererComponent>().VertexArrays[0];

  commandBuffer->Reset();
  m_SwapChain->AcquireNextImage(commandBuffer);

  commandBuffer->Begin();
  {
    m_Framebuffer->Bind(commandBuffer);
    m_Shader->Bind(commandBuffer);
    vertexArray->Bind(commandBuffer);

    commandBuffer->CmdSetViewport(m_SwapChain);
    commandBuffer->CmdSetScissor(m_SwapChain);
    commandBuffer->CmdDrawIndexed(vertexArray);

    commandBuffer->CmdDrawImGuiDrawData();
  }
  commandBuffer->End();

  commandBuffer->CmdUploadResources();
  commandBuffer->CmdDisplayImage(m_SwapChain);

  m_CurrentFrame = (m_CurrentFrame + 1) % s_MaxFramesInFlight;
}
