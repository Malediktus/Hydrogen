#pragma once

#include "../../Renderer/CommandBuffer.hpp"
#include "VulkanRenderDevice.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanSwapChain.hpp"

namespace Hydrogen::Vulkan {
class VulkanCommandBuffer : public CommandBuffer {
 public:
  VulkanCommandBuffer(const ReferencePointer<RenderDevice>& renderDevice);
  virtual ~VulkanCommandBuffer();

  virtual void Begin(const ReferencePointer<RenderPass>& renderPass, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Framebuffer>& framebuffer) override;
  virtual void End() override;

  virtual void CmdDraw(const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Shader>& shader) override;

 private:
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
};
}  // namespace Hydrogen::Vulkan
