#pragma once

#include <vulkan/vulkan.h>

#include "../../Renderer/SwapChain.hpp"
#include "VulkanRenderDevice.hpp"

namespace Hydrogen::Vulkan {
struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR Capabilities = {};
  DynamicArray<VkSurfaceFormatKHR> Formats;
  DynamicArray<VkPresentModeKHR> PresentModes;
};

class VulkanSwapChain : public SwapChain {
 public:
  VulkanSwapChain(const ReferencePointer<class RenderWindow>& window, const ReferencePointer<class RenderDevice>& renderDevice, bool verticalSync);
  virtual ~VulkanSwapChain();

  virtual void AcquireNextImage(const ReferencePointer<class CommandBuffer>& commandBuffer) override;

  static SwapChainSupportDetails QuerySwapChainSupportDetails(VkPhysicalDevice device, const ReferencePointer<class RenderWindow>& window);
  static SwapChainSupportDetails QuerySwapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);
  static VkFormat FindSupportedFormat(const DynamicArray<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);

  VkSurfaceKHR GetWindowSurface() { return m_WindowSurface; }
  VkQueue GetPresentQueue() { return m_PresentQueue; }
  VkQueueFamily GetPresentQueueFamily() { return m_PresentQueueFamily; }
  VkSwapchainKHR GetSwapChain() { return m_SwapChain; }
  VkExtent2D GetExtent() { return m_Extent; }
  VkFormat GetSwapChainImageFormat() { return m_SwapChainImageFormat; }
  const DynamicArray<VkImage>& GetImages() { return m_SwapChainImages; }
  const DynamicArray<VkImageView>& GetImageViews() { return m_SwapChainImageViews; }

  VkImageView GetDepthImageView() { return m_DepthImageView; }

 private:
  void FindPresentQueueFamily();
  void CreateSwapChain(SwapChainSupportDetails swapChainSupport, VkSurfaceFormatKHR surfaceFormat, VkPresentModeKHR presentMode, uint32_t imageCount);
  void CreateImageViews();
  void CreateDepthImage();
  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes, bool prefereVerticalSync);
  VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

  ReferencePointer<class RenderWindow> m_RenderWindow;
  ReferencePointer<class VulkanRenderDevice> m_RenderDevice;
  VkQueueFamily m_PresentQueueFamily;
  VkQueue m_PresentQueue;
  VkSurfaceKHR m_WindowSurface;
  VkSwapchainKHR m_SwapChain;
  VkExtent2D m_Extent;
  VkFormat m_SwapChainImageFormat;
  DynamicArray<VkImage> m_SwapChainImages;
  DynamicArray<VkImageView> m_SwapChainImageViews;

  VkImage m_DepthImage;
  VkDeviceMemory m_DepthImageMemory;
  VkImageView m_DepthImageView;
};
}  // namespace Hydrogen::Vulkan
