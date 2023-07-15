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
  VulkanSwapChain(const ReferencePointer<RenderDevice>& renderDevice, bool verticalSync);
  virtual ~VulkanSwapChain();

  static SwapChainSupportDetails QuerySwapChainSupportDetails(VkPhysicalDevice device);

  VkSwapchainKHR GetSwapChain() { return m_SwapChain; }
  VkExtent2D GetExtent() { return m_Extent; }
  VkFormat GetSwapChainImageFormat() { return m_SwapChainImageFormat; }

 private:
  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes, bool prefereVerticalSync);
  VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

  VkSwapchainKHR m_SwapChain;
  VkExtent2D m_Extent;
  VkFormat m_SwapChainImageFormat;
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  DynamicArray<VkImage> m_SwapChainImages;
  DynamicArray<VkImageView> m_SwapChainImageViews;
};
}  // namespace Hydrogen::Vulkan
