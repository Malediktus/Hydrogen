#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <tracy/Tracy.hpp>
#include <set>

using namespace Hydrogen::Vulkan;

namespace Hydrogen::Vulkan::Utils {
static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                          VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                          const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                          void*) {
    std::string prefix;
    switch (messageType) {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        prefix = "Vulkan validation layer (general): ";
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
        prefix = "Vulkan validation layer (spec violation): ";
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
        prefix = "Vulkan validation layer (performance): ";
        break;
    default:
        HY_INVOKE_ERROR("Invalid vulkan message type");
        break;
    }

    switch (messageSeverity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        HY_LOG_TRACE("{}{}", prefix, pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        HY_LOG_DEBUG("{}{}", prefix, pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        HY_LOG_WARN("{}{}", prefix, pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        HY_LOG_ERROR("{}{}", prefix, pCallbackData->pMessage);
        HY_INVOKE_ERROR("A vulkan error occured");
        break;
    default:
        HY_INVOKE_ERROR("Invalid vulkan message severity");
        break;
    }

    return VK_FALSE;
}
} // namespace Hydrogen::Vulkan::Utils

namespace Hydrogen::Vulkan::Functions {
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}
} // namespace Hydrogen::Vulkan::Functions

VulkanContext::VulkanContext(const ReferencePointer<RenderWindow>& window) : m_Window(window) {
}

void VulkanContext::Init(ProjectInformation clientInfo, ProjectInformation engineInfo) {
    ZoneScoped;

    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = clientInfo.ProjectName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(clientInfo.ProjectVersion.x, clientInfo.ProjectVersion.y, clientInfo.ProjectVersion.z);
    appInfo.pEngineName = engineInfo.ProjectName.c_str();
    appInfo.engineVersion = VK_MAKE_VERSION(engineInfo.ProjectVersion.x, engineInfo.ProjectVersion.y, engineInfo.ProjectVersion.z);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    DynamicArray<const char*> extensions;
#ifdef HY_DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
#ifdef HY_PLATFORM_APPLE
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#endif
    const auto requiredExtensions = m_Window->GetVulkanWindowExtensions();
    extensions.insert(extensions.end(), requiredExtensions.begin(), requiredExtensions.end());

    CreateInstance(appInfo, VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR, extensions, {"VK_LAYER_KHRONOS_validation"}, Utils::VulkanDebugCallback);
    CreateDebugMessenger(Utils::VulkanDebugCallback);
    m_WindowSurface = (VkSurfaceKHR) m_Window->GetVulkanWindowSurface();

    DynamicArray<const char*> deviceExtensions;
    deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
#ifdef HY_PLATFORM_APPLE
    deviceExtensions.push_back("VK_KHR_portability_subset");
#endif

    PickPhysicalDevice(deviceExtensions, [](VkPhysicalDevice device, VkSurfaceKHR surface, VulkanContext* context, DynamicArray<const char*> deviceExtensions) -> bool {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        DynamicArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        VkQueueFamily graphicsQueueFamily;
        VkQueueFamily presentQueueFamily;

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                graphicsQueueFamily = i;

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (presentSupport)
                presentQueueFamily = i;

            if (graphicsQueueFamily.has_value() && presentQueueFamily.has_value())
                break;

            i++;
        }

        // Check for device extension support
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        bool extensionsSupported = requiredExtensions.empty();

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = context->GetSwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
        }

        return graphicsQueueFamily.has_value() && presentQueueFamily.has_value() && extensionsSupported && swapChainAdequate;
    });
    GetQueueFamilies();

    CreateLogicalDevice(deviceExtensions, {"VK_LAYER_KHRONOS_validation"});
    vkGetDeviceQueue(m_Device, m_GraphicsQueueFamily.value(), 0, &m_GraphicsQueue);
    vkGetDeviceQueue(m_Device, m_PresentQueueFamily.value(), 0, &m_PresentQueue);

    CreateSwapChain();

    HY_LOG_INFO("Created Vulkan context");
    HY_LOG_INFO("Using Vulkan API version 1.0");
}

VulkanContext::~VulkanContext() {
    vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
    vkDestroyDevice(m_Device, nullptr);

#ifdef HY_DEBUG
    Functions::DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
#endif

    vkDestroySurfaceKHR(m_Instance, m_WindowSurface, nullptr);
    vkDestroyInstance(m_Instance, nullptr);
}

void VulkanContext::CreateInstance(VkApplicationInfo appInfo, VkInstanceCreateFlags flags, const DynamicArray<const char*> extensions,
                                   const DynamicArray<const char*> validationLayers, PFN_vkDebugUtilsMessengerCallbackEXT debugCallback) {
    // Check for extensions
    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

    DynamicArray<VkExtensionProperties> availableExtensions(availableExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

    for (auto extension : extensions) {
        bool found = false;
        for (const auto& availableExtension : availableExtensions) {
            if (!strcmp(extension, availableExtension.extensionName)) {
                found = true;
                break;
            }
        }

        if (found)
            continue;

        HY_LOG_ERROR("Vulkan extension '{}' is not supported!", extension);
        HY_INVOKE_ERROR("A vulkan extension is not supported");
    }

    // Check for validation layers
#ifdef HY_DEBUG
    uint32_t availableLayerCount;
    vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);

    DynamicArray<VkLayerProperties> availableLayers(availableLayerCount);
    vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data());

    for (auto validationLayer : validationLayers) {
        bool found = false;
        for (const auto& availableLayer : availableLayers) {
            if (!strcmp(validationLayer, availableLayer.layerName)) {
                found = true;
                break;
            }
        }

        if (found)
            continue;

        HY_LOG_ERROR("Vulkan validation layer '{}' is not supported!", validationLayer);
        HY_INVOKE_ERROR("A vulkan validation layer is not supported");
    }
#endif

    // Create instance
    VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo {};
    PopulateDebugMessengerCreateInfo(&debugMessengerCreateInfo, debugCallback);

    VkInstanceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
#ifdef HY_DEBUG
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugMessengerCreateInfo;
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
#else
    createInfo.pNext = nullptr;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
#endif
    createInfo.pApplicationInfo = &appInfo;
    createInfo.flags = flags;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VK_CHECK_ERROR(vkCreateInstance(&createInfo, nullptr, &m_Instance), "Failed to create vulkan instance");
}

void VulkanContext::CreateDebugMessenger(PFN_vkDebugUtilsMessengerCallbackEXT callback) {
#ifndef HY_DEBUG
    return;
#endif
    VkDebugUtilsMessengerCreateInfoEXT createInfo {};
    PopulateDebugMessengerCreateInfo(&createInfo, callback);
    VK_CHECK_ERROR(Functions::CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger), "Failed to create vulkan debug messenger");
}

void VulkanContext::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo, PFN_vkDebugUtilsMessengerCallbackEXT callback) {
#ifndef HY_DEBUG
    return;
#endif
    createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo->pfnUserCallback = callback;
    createInfo->pUserData = nullptr;
}

void VulkanContext::PickPhysicalDevice(DynamicArray<const char*> deviceExtensions,
                                       std::function<bool(VkPhysicalDevice, VkSurfaceKHR, VulkanContext*, DynamicArray<const char*>)> deviceRateFunction) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
    HY_ASSERT(deviceCount > 0, "Failed to find physical device with vulkan support");

    DynamicArray<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

    m_PhysicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties currentPhysicalDeviceProperties;
    size_t currentPhysicalDeviceMemorySize;

    for (const auto& device : devices) {
        if (!deviceRateFunction(device, m_WindowSurface, this, deviceExtensions))
            break;

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
        vkGetPhysicalDeviceMemoryProperties(device, &deviceMemoryProperties);

        size_t deviceMemorySize = 0;
        for (uint32_t i = 0; i < deviceMemoryProperties.memoryHeapCount; i++) {
            deviceMemorySize += deviceMemoryProperties.memoryHeaps[i].size;
        }

        if (m_PhysicalDevice == VK_NULL_HANDLE) {
            currentPhysicalDeviceProperties = deviceProperties;
            currentPhysicalDeviceMemorySize = deviceMemorySize;
            m_PhysicalDevice = device;
            continue;
        }

        if (currentPhysicalDeviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            currentPhysicalDeviceProperties = deviceProperties;
            currentPhysicalDeviceMemorySize = deviceMemorySize;
            m_PhysicalDevice = device;
            continue;
        }

        if (deviceMemorySize > currentPhysicalDeviceMemorySize) {
            currentPhysicalDeviceProperties = deviceProperties;
            currentPhysicalDeviceMemorySize = deviceMemorySize;
            m_PhysicalDevice = device;
            continue;
        }
    }

    HY_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "Failed to find suitable physical device");
}

void VulkanContext::GetQueueFamilies() {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

    DynamicArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            m_GraphicsQueueFamily = i;

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, m_WindowSurface, &presentSupport);

        if (presentSupport)
            m_PresentQueueFamily = i;

        if (m_GraphicsQueueFamily.has_value() && m_PresentQueueFamily.has_value())
            break;

        i++;
    }

    HY_ASSERT(m_GraphicsQueueFamily.has_value(), "Graphics Family can't be found");
    HY_ASSERT(m_PresentQueueFamily.has_value(), "Present Family can't be found");
}

void VulkanContext::CreateLogicalDevice(const DynamicArray<const char*> extensions, const DynamicArray<const char*> validationLayers) {
    DynamicArray<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {m_GraphicsQueueFamily.value(), m_PresentQueueFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures {};

    VkDeviceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
#ifdef HY_DEBUG
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
#endif

    VK_CHECK_ERROR(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device), "Failed to create logical vulkan device");
}

SwapChainSupportDetails VulkanContext::GetSwapChainSupport(VkPhysicalDevice physicalDevice) {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_WindowSurface, &details.Capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_WindowSurface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.Formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_WindowSurface, &formatCount, details.Formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_WindowSurface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.PresentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_WindowSurface, &presentModeCount, details.PresentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR VulkanContext::ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    // TODO: Return the next best format not the first
    return availableFormats[0];
}

VkPresentModeKHR VulkanContext::ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanContext::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        auto viewportSize = m_Window->GetViewportSize();
        VkExtent2D actualExtent = {static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y)};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void VulkanContext::CreateSwapChain() {
    SwapChainSupportDetails swapChainSupport = GetSwapChainSupport(m_PhysicalDevice);

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities);

    uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;

    if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount) {
        imageCount = swapChainSupport.Capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_WindowSurface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {m_GraphicsQueueFamily.value(), m_PresentQueueFamily.value()};

    if (m_GraphicsQueueFamily != m_PresentQueueFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VK_CHECK_ERROR(vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &m_SwapChain), "Failed to create swap chain");
}
