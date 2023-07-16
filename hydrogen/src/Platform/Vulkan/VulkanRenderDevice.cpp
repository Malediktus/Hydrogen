#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanSwapChain.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <set>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanRenderDevice::VulkanRenderDevice(std::function<std::size_t(const RenderDeviceProperties&)> deviceRateFunction) {
  ZoneScoped;
  auto instance = Renderer::GetContext<VulkanContext>()->GetInstance();
  auto validationLayers = Renderer::GetContext<VulkanContext>()->GetValidationLayers();
  auto deviceExtensions = Renderer::GetContext<VulkanContext>()->GetDeviceExtensions();

  // Physical device
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  HY_ASSERT(deviceCount, "Failed to find GPU with vulkan support!");

  DynamicArray<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  std::multimap<std::size_t, VkPhysicalDevice> candidates;

  for (auto& device : devices) {
    auto graphicsQueueFamily = GetGraphicsQueueFamily(device);
    auto presentQueueFamily = GetPresentQueueFamily(device);

    if (!graphicsQueueFamily.has_value() || !presentQueueFamily.has_value()) continue;

    if (!CheckDeviceExtensionSupport(device, deviceExtensions)) continue;

    bool swapChainAdequate = false;
    SwapChainSupportDetails swapChainSupport = VulkanSwapChain::QuerySwapChainSupportDetails(device);
    swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();

    if (!swapChainAdequate) continue;

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(device, &deviceMemoryProperties);

    RenderDeviceType renderDeviceType = RenderDeviceType::Unknown;
    switch (deviceProperties.deviceType) {
      case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        renderDeviceType = RenderDeviceType::Unknown;
        break;
      case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        renderDeviceType = RenderDeviceType::IntegratedGPU;
        break;
      case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        renderDeviceType = RenderDeviceType::DiscreteGPU;
        break;
      case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        renderDeviceType = RenderDeviceType::VirtualGPU;
        break;
      case VK_PHYSICAL_DEVICE_TYPE_CPU:
        renderDeviceType = RenderDeviceType::CPU;
        break;
      default:
        HY_INVOKE_ERROR("Unknown or wrong vulkan physical device type!");
    }

    RenderDeviceProperties renderDeviceProperties;
    renderDeviceProperties.VendorID = deviceProperties.vendorID;
    renderDeviceProperties.DeviceID = deviceProperties.deviceID;
    renderDeviceProperties.DeviceType = renderDeviceType;
    renderDeviceProperties.DeviceName = deviceProperties.deviceName;
    renderDeviceProperties.MemoryHeaps = DynamicArray<RenderDeviceHeap>();
    renderDeviceProperties.MemoryHeaps.reserve(deviceMemoryProperties.memoryHeapCount);
    renderDeviceProperties.GeometryShaderSupport = deviceFeatures.geometryShader;
    renderDeviceProperties.TesslationShaderSupport = deviceFeatures.tessellationShader;

    for (uint32_t i = 0; i < deviceMemoryProperties.memoryHeapCount; i++) {
      renderDeviceProperties.MemoryHeaps.push_back({deviceMemoryProperties.memoryHeaps->size});
    }

    candidates.insert(std::make_pair(deviceRateFunction(renderDeviceProperties), device));
  }

  if (candidates.rbegin()->first > 0) {
    m_PhysicalDevice = candidates.rbegin()->second;
  } else {
    HY_INVOKE_ERROR("Failed to find a suitable render device!");
  }

  // Queue families
  m_GraphicsQueueFamily = GetGraphicsQueueFamily(m_PhysicalDevice);
  m_PresentQueueFamily = GetPresentQueueFamily(m_PhysicalDevice);

  // Logical device
  float queuePriority = 1.0f;

  VkDeviceQueueCreateInfo graphicsQueueCreateInfo{};
  graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  graphicsQueueCreateInfo.queueFamilyIndex = m_GraphicsQueueFamily.value();
  graphicsQueueCreateInfo.queueCount = 1;
  graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;

  VkPhysicalDeviceFeatures deviceFeatures{};

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = &graphicsQueueCreateInfo;
  createInfo.queueCreateInfoCount = 1;
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions.data();
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;
#ifdef HY_DEBUG
  createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
  createInfo.ppEnabledLayerNames = validationLayers.data();
#endif

  VK_CHECK_ERROR(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device), "Failed to create vulkan device!");

  vkGetDeviceQueue(m_Device, m_GraphicsQueueFamily.value(), 0, &m_GraphicsQueue);
  vkGetDeviceQueue(m_Device, m_PresentQueueFamily.value(), 0, &m_PresentQueue);

  // Command pool
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = m_GraphicsQueueFamily.value();

  VK_CHECK_ERROR(vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool), "Failed to create vulkan command pool!");
}

VulkanRenderDevice::~VulkanRenderDevice() {
  ZoneScoped;
  vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
  vkDestroyDevice(m_Device, nullptr);
}

VkQueueFamily VulkanRenderDevice::GetGraphicsQueueFamily(VkPhysicalDevice device) {
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  DynamicArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      return i;
    }

    i++;
  }

  return VkQueueFamily();
}

VkQueueFamily VulkanRenderDevice::GetPresentQueueFamily(VkPhysicalDevice device) {
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  for (uint32_t i = 0; i < queueFamilyCount; i++) {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Renderer::GetContext<VulkanContext>()->GetWindowSurface(), &presentSupport);

    if (presentSupport) {
      return i;
    }
  }

  return VkQueueFamily();
}

bool VulkanRenderDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

  std::set<String> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

  for (const auto& extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}
