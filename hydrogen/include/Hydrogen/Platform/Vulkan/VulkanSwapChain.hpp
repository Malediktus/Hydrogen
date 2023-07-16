#pragma once

#include "../../Renderer/SwapChain.hpp"
#include "VulkanContext.hpp"
#include "VulkanRenderDevice.hpp"
#include "VulkanSemaphore.hpp"

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

  virtual void AcquireNextImage(const ReferencePointer<Semaphore>& semaphore, uint32_t* imageIndex) override;

  static SwapChainSupportDetails QuerySwapChainSupportDetails(VkPhysicalDevice device);

  VkSwapchainKHR GetSwapChain() { return m_SwapChain; }
  VkExtent2D GetExtent() { return m_Extent; }
  VkFormat GetSwapChainImageFormat() { return m_SwapChainImageFormat; }
  const DynamicArray<VkImage>& GetImages() { return m_SwapChainImages; }
  const DynamicArray<VkImageView>& GetImageViews() { return m_SwapChainImageViews; }

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
