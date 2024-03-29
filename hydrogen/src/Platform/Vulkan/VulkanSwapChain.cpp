#include <Hydrogen/Platform/Vulkan/VulkanSwapChain.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanCommandBuffer.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Core/Window.hpp>
#include <Hydrogen/Core/Base.hpp>
#include <tracy/Tracy.hpp>
#include <algorithm>

using namespace Hydrogen::Vulkan;

VulkanSwapChain::VulkanSwapChain(const ReferencePointer<RenderWindow>& window, const ReferencePointer<RenderDevice>& renderDevice, bool verticalSync)
    : m_RenderWindow(window), m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(renderDevice)) {
  ZoneScoped;

  m_WindowSurface = static_cast<VkSurfaceKHR>(window->GetVulkanWindowSurface());
  FindPresentQueueFamily();
  HY_ASSERT(m_PresentQueueFamily.has_value(), "PresentQueueFamily not found!");
  vkGetDeviceQueue(m_RenderDevice->GetDevice(), m_PresentQueueFamily.value(), 0, &m_PresentQueue);

  auto swapChainSupport = QuerySwapChainSupportDetails(m_RenderDevice->GetPhysicalDevice(), m_WindowSurface);
  auto surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
  auto presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes, verticalSync);

  uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;
  if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount) {
    imageCount = swapChainSupport.Capabilities.maxImageCount;
  }

  CreateSwapChain(swapChainSupport, surfaceFormat, presentMode, imageCount);

  vkGetSwapchainImagesKHR(m_RenderDevice->GetDevice(), m_SwapChain, &imageCount, nullptr);
  m_SwapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(m_RenderDevice->GetDevice(), m_SwapChain, &imageCount, m_SwapChainImages.data());

  CreateImageViews();
}

VulkanSwapChain::~VulkanSwapChain() {
  ZoneScoped;

  for (auto imageView : m_SwapChainImageViews) {
    vkDestroyImageView(m_RenderDevice->GetDevice(), imageView, nullptr);
  }

  vkDestroySwapchainKHR(m_RenderDevice->GetDevice(), m_SwapChain, nullptr);
  vkDestroySurfaceKHR(Renderer::GetContext<VulkanContext>()->GetInstance(), m_WindowSurface, nullptr);
}

void VulkanSwapChain::AcquireNextImage(const ReferencePointer<CommandBuffer>& commandBuffer) {
  auto vulkanCommandBuffer = std::dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer);
  vkAcquireNextImageKHR(m_RenderDevice->GetDevice(), m_SwapChain, UINT64_MAX, vulkanCommandBuffer->GetImageAvailableSemaphore(), VK_NULL_HANDLE,
                        vulkanCommandBuffer->GetImageIndexPointer());
}

void VulkanSwapChain::FindPresentQueueFamily() {
  m_PresentQueueFamily = VkQueueFamily();

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(m_RenderDevice->GetPhysicalDevice(), &queueFamilyCount, nullptr);

  for (uint32_t i = 0; i < queueFamilyCount; i++) {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_RenderDevice->GetPhysicalDevice(), i, m_WindowSurface, &presentSupport);

    if (presentSupport) {
      m_PresentQueueFamily = i;
      break;
    }
  }
}

void VulkanSwapChain::CreateSwapChain(SwapChainSupportDetails swapChainSupport, VkSurfaceFormatKHR surfaceFormat, VkPresentModeKHR presentMode, uint32_t imageCount) {
  m_Extent = ChooseSwapExtent(swapChainSupport.Capabilities);
  m_SwapChainImageFormat = surfaceFormat.format;

  VkSwapchainCreateInfoKHR swapChainCreateInfo{};
  swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapChainCreateInfo.surface = m_WindowSurface;
  swapChainCreateInfo.minImageCount = imageCount;
  swapChainCreateInfo.imageFormat = surfaceFormat.format;
  swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
  swapChainCreateInfo.imageExtent = m_Extent;
  swapChainCreateInfo.imageArrayLayers = 1;
  swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapChainCreateInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
  swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapChainCreateInfo.presentMode = presentMode;
  swapChainCreateInfo.clipped = VK_TRUE;
  swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

  VkQueueFamily graphicsFamily = m_RenderDevice->GetGraphicsQueueFamily();
  uint32_t queueFamilyIndices[] = {graphicsFamily.value(), m_PresentQueueFamily.value()};

  if (graphicsFamily != m_PresentQueueFamily) {
    swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapChainCreateInfo.queueFamilyIndexCount = 2;
    swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapChainCreateInfo.queueFamilyIndexCount = 0;      // Optional
    swapChainCreateInfo.pQueueFamilyIndices = nullptr;  // Optional
  }

  VK_CHECK_ERROR(vkCreateSwapchainKHR(m_RenderDevice->GetDevice(), &swapChainCreateInfo, nullptr, &m_SwapChain), "Failed to create vulkan swap chain!");
}

void VulkanSwapChain::CreateImageViews() {
  m_SwapChainImageViews.resize(m_SwapChainImages.size());
  for (size_t i = 0; i < m_SwapChainImages.size(); i++) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = m_SwapChainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = m_SwapChainImageFormat;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    VK_CHECK_ERROR(vkCreateImageView(m_RenderDevice->GetDevice(), &createInfo, nullptr, &m_SwapChainImageViews[i]), "Failed to create vulkan image view!");
  }
}

SwapChainSupportDetails VulkanSwapChain::QuerySwapChainSupportDetails(VkPhysicalDevice device, const ReferencePointer<RenderWindow>& window) {
  SwapChainSupportDetails details;
  VkSurfaceKHR surface = (VkSurfaceKHR)window->GetVulkanWindowSurface();

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

SwapChainSupportDetails VulkanSwapChain::QuerySwapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface) {
  SwapChainSupportDetails details;

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
    Vector2 viewportSize = m_RenderWindow->GetViewportSize();

    VkExtent2D actualExtent = {static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y)};

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
  }
}
