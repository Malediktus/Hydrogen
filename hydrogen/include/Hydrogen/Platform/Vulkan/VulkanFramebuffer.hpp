#pragma once

#include <vulkan/vulkan.h>

#include "../../Renderer/Framebuffer.hpp"

namespace Hydrogen::Vulkan {
class VulkanFramebuffer : public Framebuffer {
 public:
  VulkanFramebuffer(const ReferencePointer<class RenderDevice>& renderDevice, const ReferencePointer<class SwapChain>& swapChain);
  virtual ~VulkanFramebuffer();

  virtual void Begin(Vector4 clearColor, const ReferencePointer<class CommandBuffer>& commandBuffer) override;
  virtual void End(const ReferencePointer<class CommandBuffer>& commandBuffer) override;

  const DynamicArray<VkFramebuffer>& GetFramebuffers() { return m_Framebuffers; }
  VkRenderPass GetRenderPass() { return m_RenderPass; }

 private:
  ReferencePointer<class VulkanRenderDevice> m_RenderDevice;
  ReferencePointer<class VulkanSwapChain> m_SwapChain;
  VkRenderPass m_RenderPass;
  DynamicArray<VkFramebuffer> m_Framebuffers;
};
}  // namespace Hydrogen::Vulkan
