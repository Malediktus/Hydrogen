#pragma once

#include "../../Renderer/Fence.hpp"
#include "VulkanContext.hpp"
#include "VulkanRenderDevice.hpp"

namespace Hydrogen::Vulkan {
class VulkanFence : public Fence {
 public:
  VulkanFence(const ReferencePointer<RenderDevice>& renderDevice);
  virtual ~VulkanFence();

 private:
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  VkFence m_Fence;
};
}  // namespace Hydrogen::Vulkan
