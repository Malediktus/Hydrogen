#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanSwapChain.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanSwapChain::VulkanSwapChain() { ZoneScoped; }

VulkanSwapChain::~VulkanSwapChain() { ZoneScoped; }

SwapChainSupportDetails VulkanSwapChain::QuerySwapChainSupportDetails(
    VkPhysicalDevice device) {
  SwapChainSupportDetails details;
  auto surface = static_cast<VkSurfaceKHR>(Renderer::GetContext<VulkanContext>()
                                               ->GetWindow()
                                               ->GetVulkanWindowSurface());

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.Capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if (formatCount != 0) {
    details.Formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.Formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            nullptr);

  if (presentModeCount != 0) {
    details.PresentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.PresentModes.data());
  }

  return details;
}
