#pragma once

#include "../../Renderer/Semaphore.hpp"
#include "VulkanContext.hpp"
#include "VulkanRenderDevice.hpp"

namespace Hydrogen::Vulkan {
class VulkanSemaphore : public Semaphore {
 public:
  VulkanSemaphore(const ReferencePointer<RenderDevice>& renderDevice);
  virtual ~VulkanSemaphore();

  VkSemaphore GetSemaphore() { return m_Semaphore; }

 private:
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  VkSemaphore m_Semaphore;
};
}  // namespace Hydrogen::Vulkan
