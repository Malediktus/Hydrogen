#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanRenderDevice::VulkanRenderDevice(std::function<std::size_t(const RenderDeviceProperties&)> deviceRateFunction) {
    ZoneScoped;
    auto instance = Renderer::GetContext<VulkanContext>()->GetInstance();

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    HY_ASSERT(deviceCount, "Failed to find GPU with vulkan support!");

    DynamicArray<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (auto& device : devices) {
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
    }
}

VulkanRenderDevice::~VulkanRenderDevice() {
    ZoneScoped;
}
