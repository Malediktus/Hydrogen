#include <Hydrogen/Assets/AssetManager.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<Context> Renderer::s_Context;

Renderer::Renderer(const ReferencePointer<RenderWindow>& window, const ReferencePointer<RenderDevice>& device) {
  ZoneScoped;

  const float vertices[] = {0.0f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};

  m_SwapChain = SwapChain::Create(window, device, true);
  m_RenderPass = RenderPass::Create(device, m_SwapChain);
  m_Shader = AssetManager::Get<ShaderAsset>("assets/Raw.glsl")
                 ->CreateShader({{ShaderDataType::Float2, "Position", false}, {ShaderDataType::Float3, "Color", false}}, device, m_SwapChain, m_RenderPass);
  m_Framebuffer = Framebuffer::Create(device, m_SwapChain, m_RenderPass);
  m_CommandBuffer = CommandBuffer::Create(device);
  m_ImageAvailableSemaphore = Semaphore::Create(device);
  m_RenderFinishedSemaphore = Semaphore::Create(device);
  m_InFlightFence = Fence::Create(device);
  m_VertexBuffer = VertexBuffer::Create(device, (float*)vertices, sizeof(vertices));
  m_VertexBuffer->SetLayout({{ShaderDataType::Float2, "Position", false}, {ShaderDataType::Float3, "Color", false}});
  m_FirstFrame = true;

  HY_LOG_INFO("Initialized renderer");
}

Renderer::~Renderer() {
  if (!m_FirstFrame) {
    m_InFlightFence->Wait();
    m_InFlightFence->Reset();
  }
}

void Renderer::Render() {
  m_CommandBuffer->Reset();
  m_CommandBuffer->Begin(m_RenderPass, m_SwapChain, m_Framebuffer, {1.0f, 0.0f, 1.0f, 1.0f});
  m_Shader->Bind(m_CommandBuffer);
  m_VertexBuffer->Bind(m_CommandBuffer);
  m_CommandBuffer->CmdSetViewport(m_SwapChain);
  m_CommandBuffer->CmdDraw(m_VertexBuffer);
  m_CommandBuffer->End(m_SwapChain);

  // m_CommandBuffer->SubmitGraphicsQueue(m_ImageAvailableSemaphore, m_RenderFinishedSemaphore, m_InFlightFence);
  // m_CommandBuffer->PresentQueue(m_RenderFinishedSemaphore, m_SwapChain, &imageIndex);
}
