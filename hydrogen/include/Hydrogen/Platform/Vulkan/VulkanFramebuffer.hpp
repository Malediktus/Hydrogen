#pragma once

#include "../../Renderer/Framebuffer.hpp"
#include "VulkanRenderDevice.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanSwapChain.hpp"

namespace Hydrogen::Vulkan {
class VulkanFramebuffer : public Framebuffer {
 public:
  VulkanFramebuffer(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<RenderPass>& renderPass);
  virtual ~VulkanFramebuffer();

  const DynamicArray<VkFramebuffer>& GetFramebuffers() { return m_SwapChainFramebuffers; }

 private:
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  ReferencePointer<VulkanSwapChain> m_SwapChain;
  ReferencePointer<VulkanRenderPass> m_RenderPass;
  DynamicArray<VkFramebuffer> m_SwapChainFramebuffers;
};
}  // namespace Hydrogen::Vulkan
