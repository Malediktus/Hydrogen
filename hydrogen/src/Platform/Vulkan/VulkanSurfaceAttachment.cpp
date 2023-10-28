#include <Hydrogen/Core/Base.hpp>
#include <Hydrogen/Core/Window.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanCommandBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanSurfaceAttachment.hpp>
#include <Hydrogen/Renderer/RenderWindow.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <algorithm>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

namespace Hydrogen::Vulkan::Utils {
static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
  VkPhysicalDeviceMemoryProperties memoryProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

  for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }

  HY_INVOKE_ERROR("Failed to find suitable memory type!");
}
}  // namespace Hydrogen::Vulkan::Utils

VulkanSurfaceAttachment::VulkanSurfaceAttachment(RenderWindow* window, bool verticalSync) : m_RenderWindow(window) {
  ZoneScoped;

  m_WindowSurface = static_cast<VkSurfaceKHR>(window->GetVulkanWindowSurface());
  FindPresentQueueFamily();
  HY_ASSERT(m_PresentQueueFamily.has_value(), "PresentQueueFamily not found!");
  vkGetDeviceQueue(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_PresentQueueFamily.value(), 0, &m_PresentQueue);

  auto swapChainSupport = QuerySwapChainSupportDetails(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetPhysicalDevice(), m_WindowSurface);
  auto surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
  auto presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes, verticalSync);

  uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;
  if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount) {
    imageCount = swapChainSupport.Capabilities.maxImageCount;
  }

  CreateSwapChain(swapChainSupport, surfaceFormat, presentMode, imageCount);

  vkGetSwapchainImagesKHR(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_SwapChain, &imageCount, nullptr);
  m_SwapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_SwapChain, &imageCount, m_SwapChainImages.data());

  CreateImageViews();
  CreateDepthImage();

  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = m_SwapChainImageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL,
                                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, Renderer::GetRenderDevice<VulkanRenderDevice>()->GetPhysicalDevice());
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = 1;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcAccessMask = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  StaticArray<VkAttachmentDescription, 2> renderPassAttachments = {colorAttachment, depthAttachment};
  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = static_cast<uint32_t>(renderPassAttachments.size());
  renderPassInfo.pAttachments = renderPassAttachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  VK_CHECK_ERROR(vkCreateRenderPass(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass), "Failed to create vulkan render pass!");

  m_Framebuffers.resize(m_SwapChainImageViews.size());

  for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
    StaticArray<VkImageView, 2> attachments = {m_SwapChainImageViews[i], m_DepthImageView};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = m_RenderPass;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = m_Extent.width;
    framebufferInfo.height = m_Extent.height;
    framebufferInfo.layers = 1;

    VK_CHECK_ERROR(vkCreateFramebuffer(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &framebufferInfo, nullptr, &m_Framebuffers[i]),
                   "Failed to create vulkan framebuffer!");
  }
}

VulkanSurfaceAttachment::~VulkanSurfaceAttachment() {
  ZoneScoped;

  for (auto framebuffer : m_Framebuffers) {
    vkDestroyFramebuffer(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), framebuffer, nullptr);
  }
  vkDestroyRenderPass(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_RenderPass, nullptr);

  vkDestroyImageView(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_DepthImageView, nullptr);
  vkDestroyImage(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_DepthImage, nullptr);
  vkFreeMemory(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_DepthImageMemory, nullptr);

  for (auto imageView : m_SwapChainImageViews) {
    vkDestroyImageView(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), imageView, nullptr);
  }

  vkDestroySwapchainKHR(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_SwapChain, nullptr);
  vkDestroySurfaceKHR(Renderer::GetContext<VulkanContext>()->GetInstance(), m_WindowSurface, nullptr);
}

void VulkanSurfaceAttachment::FindPresentQueueFamily() {
  m_PresentQueueFamily = VkQueueFamily();

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetPhysicalDevice(), &queueFamilyCount, nullptr);

  for (uint32_t i = 0; i < queueFamilyCount; i++) {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetPhysicalDevice(), i, m_WindowSurface, &presentSupport);

    if (presentSupport) {
      m_PresentQueueFamily = i;
      break;
    }
  }
}

void VulkanSurfaceAttachment::CreateSwapChain(SwapChainSupportDetails swapChainSupport, VkSurfaceFormatKHR surfaceFormat, VkPresentModeKHR presentMode, uint32_t imageCount) {
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

  VkQueueFamily graphicsFamily = Renderer::GetRenderDevice<VulkanRenderDevice>()->GetGraphicsQueueFamily();
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

  VK_CHECK_ERROR(vkCreateSwapchainKHR(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &swapChainCreateInfo, nullptr, &m_SwapChain),
                 "Failed to create vulkan swap chain!");
}

void VulkanSurfaceAttachment::CreateImageViews() {
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

    VK_CHECK_ERROR(vkCreateImageView(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &createInfo, nullptr, &m_SwapChainImageViews[i]),
                   "Failed to create vulkan image view!");
  }
}

void VulkanSurfaceAttachment::CreateDepthImage() {
  auto depthFormat = FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL,
                                         VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, Renderer::GetRenderDevice<VulkanRenderDevice>()->GetPhysicalDevice());

  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = m_Extent.width;
  imageInfo.extent.height = m_Extent.height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = depthFormat;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateImage(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &imageInfo, nullptr, &m_DepthImage) != VK_SUCCESS) {
    HY_INVOKE_ERROR("Failed to create image!");
  }

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_DepthImage, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      Utils::FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, Renderer::GetRenderDevice<VulkanRenderDevice>()->GetPhysicalDevice());

  if (vkAllocateMemory(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &allocInfo, nullptr, &m_DepthImageMemory) != VK_SUCCESS) {
    HY_INVOKE_ERROR("Failed to allocate image memory!");
  }

  vkBindImageMemory(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_DepthImage, m_DepthImageMemory, 0);

  VkImageViewCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  createInfo.image = m_DepthImage;
  createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  createInfo.format = depthFormat;
  createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  createInfo.subresourceRange.baseMipLevel = 0;
  createInfo.subresourceRange.levelCount = 1;
  createInfo.subresourceRange.baseArrayLayer = 0;
  createInfo.subresourceRange.layerCount = 1;

  VK_CHECK_ERROR(vkCreateImageView(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &createInfo, nullptr, &m_DepthImageView), "Failed to create vulkan image view!");
}

SwapChainSupportDetails VulkanSurfaceAttachment::QuerySwapChainSupportDetails(VkPhysicalDevice device, const ReferencePointer<RenderWindow>& window) {
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

SwapChainSupportDetails VulkanSurfaceAttachment::QuerySwapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface) {
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

VkSurfaceFormatKHR VulkanSurfaceAttachment::ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats) {
  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR VulkanSurfaceAttachment::ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes, bool prefereVerticalSync) {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR && !prefereVerticalSync) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSurfaceAttachment::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
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

VkFormat VulkanSurfaceAttachment::FindSupportedFormat(const DynamicArray<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features,
                                                      VkPhysicalDevice physicalDevice) {
  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  HY_INVOKE_ERROR("Failed to find supported format!");
}
