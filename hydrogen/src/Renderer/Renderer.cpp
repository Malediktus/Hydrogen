#include <Hydrogen/Assets/AssetManager.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<Context> Renderer::s_Context;

Renderer::Renderer(const ReferencePointer<RenderWindow>& window, const ReferencePointer<RenderDevice>& device) {
  ZoneScoped;

  m_SwapChain = SwapChain::Create(window, device, true);
  m_RenderPass = RenderPass::Create(device, m_SwapChain);
  m_Shader = AssetManager::Get<ShaderAsset>("assets/Raw.glsl")->CreateShader(device, m_SwapChain, m_RenderPass);
  m_Framebuffer = Framebuffer::Create(device, m_SwapChain, m_RenderPass);
  m_CommandBuffer = CommandBuffer::Create(device);
  m_ImageAvailableSemaphore = Semaphore::Create(device);
  m_RenderFinishedSemaphore = Semaphore::Create(device);
  m_InFlightFence = Fence::Create(device);
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
  if (!m_FirstFrame) {
    m_InFlightFence->Wait();
    m_InFlightFence->Reset();
  }
  m_FirstFrame = false;

  uint32_t imageIndex;
  m_SwapChain->AcquireNextImage(m_ImageAvailableSemaphore, &imageIndex);

  m_CommandBuffer->Reset();
  m_CommandBuffer->Begin(m_RenderPass, m_SwapChain, m_Framebuffer, {1.0f, 0.0f, 1.0f, 1.0f}, imageIndex);
  m_CommandBuffer->CmdDraw(m_SwapChain, m_Shader);
  m_CommandBuffer->End();

  m_CommandBuffer->SubmitGraphicsQueue(m_ImageAvailableSemaphore, m_RenderFinishedSemaphore, m_InFlightFence);
  m_CommandBuffer->PresentQueue(m_RenderFinishedSemaphore, m_SwapChain, &imageIndex);
}
