#pragma once

#include "../../Renderer/CommandBuffer.hpp"
#include "VulkanFence.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanRenderDevice.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanShader.hpp"
#include "VulkanSwapChain.hpp"

namespace Hydrogen::Vulkan {
class VulkanCommandBuffer : public CommandBuffer {
 public:
  VulkanCommandBuffer(const ReferencePointer<RenderDevice>& renderDevice);
  virtual ~VulkanCommandBuffer();

  virtual void Reset() override;
  virtual void Begin(const ReferencePointer<RenderPass>& renderPass, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Framebuffer>& framebuffer,
                     Vector4 clearColor) override;
  virtual void End(const ReferencePointer<SwapChain> swapChain) override;

  virtual void CmdDraw(const ReferencePointer<VertexBuffer>& vertexBuffer) override;
  virtual void CmdSetViewport(const ReferencePointer<SwapChain>& swapChain, uint32_t width = UINT32_MAX, uint32_t height = UINT32_MAX) override;

  VkCommandBuffer GetCommandBuffer() { return m_CommandBuffer; }

 private:
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  VkCommandBuffer m_CommandBuffer;
  VkSemaphore m_ImageAvailableSemaphore;
  VkSemaphore m_RenderFinishedSemaphore;
  VkFence m_InFlightFence;
  uint32_t m_ImageIndex;
};
}  // namespace Hydrogen::Vulkan
