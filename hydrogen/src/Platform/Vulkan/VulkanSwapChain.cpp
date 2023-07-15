#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanSwapChain.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanSwapChain::VulkanSwapChain(ReferencePointer<RenderDevice> renderDevice, bool verticalSync) : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(renderDevice)) {
  ZoneScoped;

  SwapChainSupportDetails swapChainSupport = QuerySwapChainSupportDetails(m_RenderDevice->GetPhysicalDevice());

  VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
  VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes, verticalSync);
  VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities);

  uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;
  if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount) {
    imageCount = swapChainSupport.Capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = Renderer::GetContext<VulkanContext>()->GetWindowSurface();
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  createInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  VkQueueFamily graphicsFamily = m_RenderDevice->GetGraphicsQueueFamily();
  VkQueueFamily presentFamily = m_RenderDevice->GetPresentQueueFamily();
  uint32_t queueFamilyIndices[] = {graphicsFamily.value(), presentFamily.value()};

  if (graphicsFamily != presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;      // Optional
    createInfo.pQueueFamilyIndices = nullptr;  // Optional
  }

  VK_CHECK_ERROR(vkCreateSwapchainKHR(m_RenderDevice->GetDevice(), &createInfo, nullptr, &m_SwapChain), "Failed to create vulkan swap chain!");
}

VulkanSwapChain::~VulkanSwapChain() {
  ZoneScoped;
  vkDestroySwapchainKHR(m_RenderDevice->GetDevice(), m_SwapChain, nullptr);
}

SwapChainSupportDetails VulkanSwapChain::QuerySwapChainSupportDetails(VkPhysicalDevice device) {
  SwapChainSupportDetails details;
  auto surface = Renderer::GetContext<VulkanContext>()->GetWindowSurface();

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if (formatCount != 0) {
    details.Formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.Formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

  if (presentModeCount != 0) {
    details.PresentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.PresentModes.data());
  }

  return details;
}

VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats) {
  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes, bool prefereVerticalSync) {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR && !prefereVerticalSync) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    Vector2 viewportSize = Renderer::GetContext<VulkanContext>()->GetWindow()->GetViewportSize();

    VkExtent2D actualExtent = {static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y)};

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
  }
}
