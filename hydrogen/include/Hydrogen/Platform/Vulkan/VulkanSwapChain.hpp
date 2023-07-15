#include "../../Renderer/SwapChain.hpp"
#include "VulkanContext.hpp"

namespace Hydrogen::Vulkan {
struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR Capabilities;
  DynamicArray<VkSurfaceFormatKHR> Formats;
  DynamicArray<VkPresentModeKHR> PresentModes;
};

class VulkanSwapChain : public SwapChain {
 public:
  VulkanSwapChain();
  virtual ~VulkanSwapChain();

  static SwapChainSupportDetails QuerySwapChainSupportDetails(VkPhysicalDevice device);

 private:
};
}  // namespace Hydrogen::Vulkan
