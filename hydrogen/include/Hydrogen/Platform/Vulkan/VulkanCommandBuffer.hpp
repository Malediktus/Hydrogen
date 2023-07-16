#pragma once

#include "../../Renderer/CommandBuffer.hpp"
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

  virtual void Begin(const ReferencePointer<RenderPass>& renderPass, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Framebuffer>& framebuffer,
                     Vector4 clearColor, uint32_t imageIndex) override;
  virtual void End() override;

  virtual void CmdDraw(const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Shader>& shader) override;

 private:
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  VkCommandBuffer m_CommandBuffer;
};
}  // namespace Hydrogen::Vulkan
