#pragma once

#include <vulkan/vulkan.h>

#include <cstdint>

#include "../../Core/Memory.hpp"
#include "../../Renderer/CommandBuffer.hpp"

namespace Hydrogen::Vulkan {
class VulkanCommandBuffer : public CommandBuffer {
 public:
  VulkanCommandBuffer(const ReferencePointer<class RenderDevice>& renderDevice);
  virtual ~VulkanCommandBuffer();

  virtual void Reset() override;
  virtual void Begin() override;
  virtual void End() override;

  virtual void CmdUploadResources() override;
  virtual void CmdDisplayImage(const ReferencePointer<class SwapChain> swapChain) override;
  virtual void CmdDraw(const ReferencePointer<class VertexBuffer>& vertexBuffer) override;
  virtual void CmdDrawIndexed(const ReferencePointer<class VertexArray>& vertexArray) override;
  virtual void CmdSetViewport(const ReferencePointer<class SwapChain>& swapChain, uint32_t width = UINT32_MAX, uint32_t height = UINT32_MAX) override;
  virtual void CmdSetScissor(const ReferencePointer<class SwapChain>& swapChain, int offsetX = 0, int offsetY = 0) override;
  virtual void CmdDrawImGuiDrawData(const ReferencePointer<class Shader>& shader = nullptr) override;

  VkCommandBuffer GetCommandBuffer() { return m_CommandBuffer; }
  VkSemaphore GetImageAvailableSemaphore() { return m_ImageAvailableSemaphore; }
  VkSemaphore GetRenderFinishedSemaphore() { return m_RenderFinishedSemaphore; }
  VkFence GetInFlightFence() { return m_InFlightFence; }
  uint32_t GetImageIndex() { return m_ImageIndex; }
  uint32_t* GetImageIndexPointer() { return &m_ImageIndex; }

 private:
  ReferencePointer<class VulkanRenderDevice> m_RenderDevice;
  VkCommandBuffer m_CommandBuffer;
  VkSemaphore m_ImageAvailableSemaphore;
  VkSemaphore m_RenderFinishedSemaphore;
  VkFence m_InFlightFence;
  uint32_t m_ImageIndex;
};
}  // namespace Hydrogen::Vulkan
