#pragma once

#include "../../Renderer/Framebuffer.hpp"

#include <vulkan/vulkan.h>

namespace Hydrogen::Vulkan {
class VulkanFramebuffer : public Framebuffer {
 public:
  VulkanFramebuffer(const ReferencePointer<class RenderDevice>& renderDevice, const ReferencePointer<class SwapChain>& swapChain);
  virtual ~VulkanFramebuffer();

  virtual void Bind(const ReferencePointer<class CommandBuffer>& commandBuffer) override;
  virtual const Vector4& GetClearColor() const override { return m_ClearColor; }
  virtual void SetClearColor(const Vector4& color) override { m_ClearColor = color; }

  const DynamicArray<VkFramebuffer>& GetFramebuffers() { return m_Framebuffers; }
  VkRenderPass GetRenderPass() { return m_RenderPass; }

 private:
  Vector4 m_ClearColor;
  ReferencePointer<class VulkanRenderDevice> m_RenderDevice;
  ReferencePointer<class VulkanSwapChain> m_SwapChain;
  VkRenderPass m_RenderPass;
  DynamicArray<VkFramebuffer> m_Framebuffers;
};
}  // namespace Hydrogen::Vulkan
