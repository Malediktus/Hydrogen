#pragma once

#include "../../Renderer/RenderDevice.hpp"
#include "VulkanContext.hpp"
#include <optional>

namespace Hydrogen::Vulkan {
using VkQueueFamily = std::optional<uint32_t>;

class VulkanRenderDevice : public RenderDevice {
 public:
  VulkanRenderDevice(std::function<std::size_t(const RenderDeviceProperties&)> deviceRateFunction);
  virtual ~VulkanRenderDevice();

  virtual bool ScreenSupported(const ReferencePointer<RenderWindow>& window) override;
  virtual void WaitForIdle() override;

  VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
  VkDevice GetDevice() { return m_Device; }
  VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
  VkQueueFamily GetGraphicsQueueFamily() { return m_GraphicsQueueFamily; }
  VkCommandPool GetCommandPool() { return m_CommandPool; }

 private:
  void PickPhysicalDevice(const DynamicArray<char*>& requiredExtensions, const std::function<std::size_t(const RenderDeviceProperties&)>& deviceRateFunction);
  void PopulateRenderDeviceProperties(RenderDeviceProperties& renderDeviceProperties, VkPhysicalDevice device);
  void CreateLogicalDevice(const DynamicArray<char*>& requiredExtensions, const DynamicArray<char*>& validationLayers);
  void CreateCommandPool();

  VkQueueFamily FindGraphicsQueueFamily(VkPhysicalDevice device);
  bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const DynamicArray<char*>& deviceExtensions);

  VkPhysicalDevice m_PhysicalDevice;
  VkQueueFamily m_GraphicsQueueFamily;
  VkDevice m_Device;
  VkQueue m_GraphicsQueue;
  VkCommandPool m_CommandPool;
};
}  // namespace Hydrogen::Vulkan
