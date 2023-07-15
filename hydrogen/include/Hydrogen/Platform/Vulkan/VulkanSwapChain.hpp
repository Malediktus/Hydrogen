#pragma once

#include "../../Renderer/SwapChain.hpp"
#include "VulkanContext.hpp"
#include "VulkanRenderDevice.hpp"

namespace Hydrogen::Vulkan {
struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR Capabilities;
  DynamicArray<VkSurfaceFormatKHR> Formats;
  DynamicArray<VkPresentModeKHR> PresentModes;
};

class VulkanSwapChain : public SwapChain {
 public:
  VulkanSwapChain(ReferencePointer<RenderDevice> renderDevice, bool verticalSync);
  virtual ~VulkanSwapChain();

  static SwapChainSupportDetails QuerySwapChainSupportDetails(VkPhysicalDevice device);

 private:
  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes, bool prefereVerticalSync);
  VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  VkSwapchainKHR m_SwapChain;
};
}  // namespace Hydrogen::Vulkan
