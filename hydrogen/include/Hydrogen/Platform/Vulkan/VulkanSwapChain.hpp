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
  VulkanSwapChain(const ReferencePointer<RenderWindow>& window, const ReferencePointer<RenderDevice>& renderDevice, bool verticalSync);
  virtual ~VulkanSwapChain();

  virtual void AcquireNextImage(const ReferencePointer<Semaphore>& semaphore, uint32_t* imageIndex) override;

  static SwapChainSupportDetails QuerySwapChainSupportDetails(VkPhysicalDevice device, const ReferencePointer<RenderWindow>& window);
  static SwapChainSupportDetails QuerySwapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

  VkSurfaceKHR GetWindowSurface() { return m_WindowSurface; }
  VkQueue GetPresentQueue() { return m_PresentQueue; }
  VkQueueFamily GetPresentQueueFamily() { return m_PresentQueueFamily; }
  VkSwapchainKHR GetSwapChain() { return m_SwapChain; }
  VkExtent2D GetExtent() { return m_Extent; }
  VkFormat GetSwapChainImageFormat() { return m_SwapChainImageFormat; }
  const DynamicArray<VkImage>& GetImages() { return m_SwapChainImages; }
  const DynamicArray<VkImageView>& GetImageViews() { return m_SwapChainImageViews; }

 private:
  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes, bool prefereVerticalSync);
  VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

  VkQueue m_PresentQueue;
  VkQueueFamily m_PresentQueueFamily;
  VkSurfaceKHR m_WindowSurface;
  VkSwapchainKHR m_SwapChain;
  VkExtent2D m_Extent;
  VkFormat m_SwapChainImageFormat;
  ReferencePointer<RenderWindow> m_RenderWindow;
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  DynamicArray<VkImage> m_SwapChainImages;
  DynamicArray<VkImageView> m_SwapChainImageViews;
};
}  // namespace Hydrogen::Vulkan
