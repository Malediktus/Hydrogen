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

 private:
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  ReferencePointer<VulkanSwapChain> m_SwapChain;
};
}  // namespace Hydrogen::Vulkan
