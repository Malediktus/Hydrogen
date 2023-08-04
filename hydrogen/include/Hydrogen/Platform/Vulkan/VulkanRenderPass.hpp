#pragma once

#include "../../Renderer/RenderPass.hpp"
#include "VulkanContext.hpp"
#include "VulkanRenderDevice.hpp"
#include "VulkanSwapChain.hpp"

namespace Hydrogen::Vulkan {
class VulkanRenderPass : public RenderPass {
 public:
  VulkanRenderPass(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain);
  virtual ~VulkanRenderPass();

  virtual void Begin(const ReferencePointer<CommandBuffer>& commandBuffer, const ReferencePointer<Framebuffer>& framebuffer, Vector4 clearColor) override;
  VkRenderPass GetRenderPass() { return m_RenderPass; }

 private:
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  ReferencePointer<VulkanSwapChain> m_SwapChain;
  VkRenderPass m_RenderPass;
};
}  // namespace Hydrogen::Vulkan
