#include "../../Renderer/RenderDevice.hpp"
#include "VulkanContext.hpp"

namespace Hydrogen::Vulkan {
using VkQueueFamily = std::optional<uint32_t>;

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR Capabilities;
  DynamicArray<VkSurfaceFormatKHR> Formats;
  DynamicArray<VkPresentModeKHR> PresentModes;
};

class VulkanRenderDevice : public RenderDevice {
 public:
  VulkanRenderDevice(std::function<std::size_t(const RenderDeviceProperties&)>
                         deviceRateFunction);
  virtual ~VulkanRenderDevice();

  VkDevice GetDevice() { return m_Device; }
  VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
  VkQueue GetPresentQueue() { return m_PresentQueue; }

 private:
  VkQueueFamily GetGraphicsQueueFamily(VkPhysicalDevice device);
  VkQueueFamily GetPresentQueueFamily(VkPhysicalDevice device);

  VkPhysicalDevice m_PhysicalDevice;
  VkQueueFamily m_GraphicsQueueFamily;
  VkQueueFamily m_PresentQueueFamily;
  VkDevice m_Device;
  VkQueue m_GraphicsQueue;
  VkQueue m_PresentQueue;
};
}  // namespace Hydrogen::Vulkan
