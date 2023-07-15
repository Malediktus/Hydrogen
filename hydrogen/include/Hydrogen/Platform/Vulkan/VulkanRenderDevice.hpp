#pragma once

#include "../../Renderer/RenderDevice.hpp"
#include "VulkanContext.hpp"

namespace Hydrogen::Vulkan {
using VkQueueFamily = std::optional<uint32_t>;

class VulkanRenderDevice : public RenderDevice {
 public:
  VulkanRenderDevice(std::function<std::size_t(const RenderDeviceProperties&)> deviceRateFunction);
  virtual ~VulkanRenderDevice();

  VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
  VkDevice GetDevice() { return m_Device; }
  VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
  VkQueue GetPresentQueue() { return m_PresentQueue; }
  VkQueueFamily GetGraphicsQueueFamily() { return m_GraphicsQueueFamily; }
  VkQueueFamily GetPresentQueueFamily() { return m_PresentQueueFamily; }

 private:
  VkQueueFamily GetGraphicsQueueFamily(VkPhysicalDevice device);
  VkQueueFamily GetPresentQueueFamily(VkPhysicalDevice device);
  bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions);

  VkPhysicalDevice m_PhysicalDevice;
  VkQueueFamily m_GraphicsQueueFamily;
  VkQueueFamily m_PresentQueueFamily;
  VkDevice m_Device;
  VkQueue m_GraphicsQueue;
  VkQueue m_PresentQueue;
};
}  // namespace Hydrogen::Vulkan
