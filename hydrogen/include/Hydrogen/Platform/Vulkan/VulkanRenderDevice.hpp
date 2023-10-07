#pragma once

#include "../../Renderer/RenderDevice.hpp"
#include <vulkan/vulkan.h>
#include <optional>
#include <functional>

namespace Hydrogen::Vulkan {
using VkQueueFamily = std::optional<uint32_t>;

class VulkanRenderDevice : public RenderDevice {
 public:
  VulkanRenderDevice(std::function<std::size_t(const RenderDeviceProperties&)> deviceRateFunction);
  virtual ~VulkanRenderDevice();

  virtual bool ScreenSupported(const ReferencePointer<class RenderWindow>& window) override;
  virtual void WaitForIdle() override;

  VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
  VkQueueFamily GetGraphicsQueueFamily() { return m_GraphicsQueueFamily; }
  VkQueueFamily GetTransferQueueFamily() { return m_TransferQueueFamily; }
  VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
  VkQueue GetTransferQueue() { return m_TransferQueue; }
  VkDevice GetDevice() { return m_Device; }
  VkCommandPool GetCommandPool() { return m_CommandPool; }

 private:
  void PickPhysicalDevice(const DynamicArray<char*>& requiredExtensions, const std::function<std::size_t(const RenderDeviceProperties&)>& deviceRateFunction);
  void PopulateRenderDeviceProperties(RenderDeviceProperties& renderDeviceProperties, VkPhysicalDevice device);
  void CreateLogicalDevice(const DynamicArray<char*>& requiredExtensions, const DynamicArray<char*>& validationLayers);
  void CreateCommandPool();

  VkQueueFamily FindGraphicsQueueFamily(VkPhysicalDevice device);
  VkQueueFamily FindTransferQueueFamily(VkPhysicalDevice device);
  bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const DynamicArray<char*>& deviceExtensions);

  VkPhysicalDevice m_PhysicalDevice;
  VkQueueFamily m_GraphicsQueueFamily;
  VkQueueFamily m_TransferQueueFamily;
  VkQueue m_GraphicsQueue;
  VkQueue m_TransferQueue;
  VkDevice m_Device;
  VkCommandPool m_CommandPool;
};
}  // namespace Hydrogen::Vulkan
