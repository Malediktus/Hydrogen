#pragma once

#include "../../Renderer/Fence.hpp"
#include "VulkanContext.hpp"
#include "VulkanRenderDevice.hpp"

namespace Hydrogen::Vulkan {
class VulkanFence : public Fence {
 public:
  VulkanFence(const ReferencePointer<RenderDevice>& renderDevice);
  virtual ~VulkanFence();

  virtual void Wait() override;
  virtual void Reset() override;

  VkFence GetFence() { return m_Fence; }

 private:
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  VkFence m_Fence;
};
}  // namespace Hydrogen::Vulkan
