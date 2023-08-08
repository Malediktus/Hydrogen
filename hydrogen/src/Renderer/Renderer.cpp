#include <Hydrogen/Assets/AssetManager.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<Context> Renderer::s_Context;
uint32_t Renderer::s_MaxFramesInFlight = 3;

Renderer::Renderer(const ReferencePointer<RenderWindow>& window, const ReferencePointer<RenderDevice>& device)
  : m_Device(device) {
  ZoneScoped;

  const float vertices[] = {0.0f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};

  m_SwapChain = SwapChain::Create(window, device, true);
  m_Framebuffer = Framebuffer::Create(device, m_SwapChain);
  m_Shader = AssetManager::Get<ShaderAsset>("assets/Raw.glsl")
                 ->CreateShader({{ShaderDataType::Float2, "Position", false}, {ShaderDataType::Float3, "Color", false}}, device, m_SwapChain, m_Framebuffer);

  m_CommandBuffers.resize(s_MaxFramesInFlight);
  for (uint32_t i = 0; i < s_MaxFramesInFlight; i++)
  {
    m_CommandBuffers[i] = CommandBuffer::Create(device);
  }

  m_VertexBuffer = VertexBuffer::Create(device, (float*)vertices, sizeof(vertices));
  m_VertexBuffer->SetLayout({{ShaderDataType::Float2, "Position", false}, {ShaderDataType::Float3, "Color", false}});
  m_CurrentFrame = 0;

  HY_LOG_INFO("Initialized renderer");
}

Renderer::~Renderer() { m_Device->WaitForIdle(); }

void Renderer::Render() {
  const auto& commandBuffer = m_CommandBuffers[m_CurrentFrame];

  commandBuffer->Reset();
  m_SwapChain->AcquireNextImage(commandBuffer);

  commandBuffer->Begin();
  {
    m_Framebuffer->Bind(commandBuffer);
    m_Shader->Bind(commandBuffer);
    m_VertexBuffer->Bind(commandBuffer);

    commandBuffer->CmdSetViewport(m_SwapChain);
    commandBuffer->CmdSetScissor(m_SwapChain);
    commandBuffer->CmdDraw(m_VertexBuffer);
  }
  commandBuffer->End();

  commandBuffer->CmdUploadResources();
  commandBuffer->CmdDisplayImage(m_SwapChain);

  m_CurrentFrame = (m_CurrentFrame + 1) % s_MaxFramesInFlight;
}
