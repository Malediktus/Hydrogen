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
  auto validationLayers = Renderer::GetContext<VulkanContext>()->GetValidationLayers();
  auto deviceExtensions = Renderer::GetContext<VulkanContext>()->GetDeviceExtensions();

  PickPhysicalDevice(deviceExtensions, deviceRateFunction);
  m_GraphicsQueueFamily = FindGraphicsQueueFamily(m_PhysicalDevice);
  m_TransferQueueFamily = FindTransferQueueFamily(m_PhysicalDevice);
  CreateLogicalDevice(deviceExtensions, validationLayers);
  vkGetDeviceQueue(m_Device, m_GraphicsQueueFamily.value(), 0, &m_GraphicsQueue);
  vkGetDeviceQueue(m_Device, m_TransferQueueFamily.value(), 0, &m_TransferQueue);
  CreateCommandPool();
}

VulkanRenderDevice::~VulkanRenderDevice() {
  ZoneScoped;
  vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
  vkDestroyDevice(m_Device, nullptr);
}

bool VulkanRenderDevice::ScreenSupported(const ReferencePointer<RenderWindow>& window) {
  HY_ASSERT(m_PhysicalDevice, "Constructor must be called before VulkanRenderDevice::ScreenSupported!")

  SwapChainSupportDetails swapChainSupport = VulkanSwapChain::QuerySwapChainSupportDetails(m_PhysicalDevice, window);
  VkQueueFamily presentFamily = VkQueueFamily();

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

  for (uint32_t i = 0; i < queueFamilyCount; i++) {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, (VkSurfaceKHR)window->GetVulkanWindowSurface(), &presentSupport);

    if (presentSupport) {
      presentFamily = i;
      break;
    }
  }

  return swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty() && presentFamily;
}

void VulkanRenderDevice::WaitForIdle() { vkDeviceWaitIdle(m_Device); }

void VulkanRenderDevice::PickPhysicalDevice(const DynamicArray<char*>& requiredExtensions,
                                            const std::function<std::size_t(const RenderDeviceProperties&)>& deviceRateFunction) {
  auto instance = Renderer::GetContext<VulkanContext>()->GetInstance();

  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  HY_ASSERT(deviceCount, "Failed to find GPU with vulkan support!");

  DynamicArray<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  std::multimap<std::size_t, VkPhysicalDevice> candidates;

  for (auto& device : devices) {
    auto graphicsQueueFamily = FindGraphicsQueueFamily(device);
    if (!graphicsQueueFamily.has_value()) continue;
    auto transferQueueFamily = FindTransferQueueFamily(device);
    if (!transferQueueFamily.has_value()) continue;
    if (!CheckDeviceExtensionSupport(device, requiredExtensions)) continue;

    RenderDeviceProperties renderDeviceProperties;
    PopulateRenderDeviceProperties(renderDeviceProperties, device);
    candidates.insert(std::make_pair(deviceRateFunction(renderDeviceProperties), device));
  }

  if (candidates.rbegin()->first > 0) {
    m_PhysicalDevice = candidates.rbegin()->second;
  } else {
    HY_INVOKE_ERROR("Failed to find a suitable render device!");
  }
}

void VulkanRenderDevice::PopulateRenderDeviceProperties(RenderDeviceProperties& renderDeviceProperties, VkPhysicalDevice device) {
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
}

void VulkanRenderDevice::CreateLogicalDevice(const DynamicArray<char*>& requiredExtensions, const DynamicArray<char*>& validationLayers) {
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
  createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
  createInfo.ppEnabledExtensionNames = requiredExtensions.data();
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;
#ifdef HY_DEBUG
  createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
  createInfo.ppEnabledLayerNames = validationLayers.data();
#endif

  VK_CHECK_ERROR(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device), "Failed to create vulkan device!");
}

void VulkanRenderDevice::CreateCommandPool() {
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = m_GraphicsQueueFamily.value();

  VK_CHECK_ERROR(vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool), "Failed to create vulkan command pool!");
}

VkQueueFamily VulkanRenderDevice::FindGraphicsQueueFamily(VkPhysicalDevice device) {
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

VkQueueFamily VulkanRenderDevice::FindTransferQueueFamily(VkPhysicalDevice device) {
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  DynamicArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
      return i;
    }

    i++;
  }

  return VkQueueFamily();
}

bool VulkanRenderDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device, const DynamicArray<char*>& deviceExtensions) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

  DynamicArray<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

  std::set<String> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

  for (const auto& extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}
