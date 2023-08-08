#pragma once

#include "../../Renderer/CommandBuffer.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanRenderDevice.hpp"
#include "VulkanShader.hpp"
#include "VulkanSwapChain.hpp"

namespace Hydrogen::Vulkan {
class VulkanCommandBuffer : public CommandBuffer {
 public:
  VulkanCommandBuffer(const ReferencePointer<RenderDevice>& renderDevice);
  virtual ~VulkanCommandBuffer();

  virtual void Reset() override;
  virtual void Begin() override;
  virtual void End() override;

  virtual void CmdUploadResources() override;
  virtual void CmdDisplayImage(const ReferencePointer<SwapChain> swapChain) override;
  virtual void CmdDraw(const ReferencePointer<VertexBuffer>& vertexBuffer) override;
  virtual void CmdSetViewport(const ReferencePointer<SwapChain>& swapChain, uint32_t width = UINT32_MAX, uint32_t height = UINT32_MAX) override;
  virtual void CmdSetScissor(const ReferencePointer<SwapChain>& swapChain, int offsetX = 0, int offsetY = 0) override;

  VkCommandBuffer GetCommandBuffer() { return m_CommandBuffer; }
  VkSemaphore GetImageAvailableSemaphore() { return m_ImageAvailableSemaphore; }
  VkSemaphore GetRenderFinishedSemaphore() { return m_RenderFinishedSemaphore; }
  VkFence GetInFlightFence() { return m_InFlightFence; }
  uint32_t GetImageIndex() { return m_ImageIndex; }
  uint32_t* GetImageIndexPointer() { return &m_ImageIndex; }

 private:
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  VkCommandBuffer m_CommandBuffer;
  VkSemaphore m_ImageAvailableSemaphore;
  VkSemaphore m_RenderFinishedSemaphore;
  VkFence m_InFlightFence;
  uint32_t m_ImageIndex;
};
}  // namespace Hydrogen::Vulkan
