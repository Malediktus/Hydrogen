#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanRenderDevice::VulkanRenderDevice(
    std::function<std::size_t(const RenderDeviceProperties&)>
        deviceRateFunction) {
  ZoneScoped;
  auto instance = Renderer::GetContext<VulkanContext>()->GetInstance();

  // Physical device
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  HY_ASSERT(deviceCount, "Failed to find GPU with vulkan support!");

  DynamicArray<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  std::multimap<std::size_t, VkPhysicalDevice> candidates;

  for (auto& device : devices) {
    auto graphicsQueueFamily = GetGraphicsQueueFamily(m_PhysicalDevice);
    auto presentQueueFamily = GetPresentQueueFamily(m_PhysicalDevice);

    if (!graphicsQueueFamily.has_value() || !presentQueueFamily.has_value())
      continue;

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
    renderDeviceProperties.MemoryHeaps.reserve(
        deviceMemoryProperties.memoryHeapCount);
    renderDeviceProperties.GeometryShaderSupport =
        deviceFeatures.geometryShader;
    renderDeviceProperties.TesslationShaderSupport =
        deviceFeatures.tessellationShader;

    for (uint32_t i = 0; i < deviceMemoryProperties.memoryHeapCount; i++) {
      renderDeviceProperties.MemoryHeaps.push_back(
          {deviceMemoryProperties.memoryHeaps->size});
    }

    candidates.insert(
        std::make_pair(deviceRateFunction(renderDeviceProperties), device));
  }

  if (candidates.rbegin()->first > 0) {
    m_PhysicalDevice = candidates.rbegin()->second;
  } else {
    HY_INVOKE_ERROR("Failed to find a suitable render device!");
  }

  // Queue families
  m_GraphicsQueueFamily = GetGraphicsQueueFamily(m_PhysicalDevice);
  m_PresentQueueFamily = GetPresentQueueFamily(m_PhysicalDevice);
}

VulkanRenderDevice::~VulkanRenderDevice() { ZoneScoped; }

VkQueueFamily VulkanRenderDevice::GetGraphicsQueueFamily(
    VkPhysicalDevice device) {
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  DynamicArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      return i;
    }

    i++;
  }

  return VkQueueFamily();
}

VkQueueFamily VulkanRenderDevice::GetPresentQueueFamily(
    VkPhysicalDevice device) {
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  for (uint32_t i = 0; i < queueFamilyCount; i++) {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(
        device, i,
        static_cast<VkSurfaceKHR>(Renderer::GetContext<VulkanContext>()
                                      ->GetWindow()
                                      ->GetVulkanWindowSurface()),
        &presentSupport);

    if (presentSupport) {
      return i;
    }
  }

  return VkQueueFamily();
}
