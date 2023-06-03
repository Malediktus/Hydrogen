#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <tracy/Tracy.hpp>

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

VulkanContext::VulkanContext(const ReferencePointer<RenderWindow>& window, ProjectInformation clientInfo, ProjectInformation engineInfo) : m_Window(window) {
    ZoneScoped;

    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = clientInfo.ProjectName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(clientInfo.ProjectVersion.x, clientInfo.ProjectVersion.y, clientInfo.ProjectVersion.z);
    appInfo.pEngineName = engineInfo.ProjectName.c_str();
    appInfo.engineVersion = VK_MAKE_VERSION(engineInfo.ProjectVersion.x, engineInfo.ProjectVersion.y, engineInfo.ProjectVersion.z);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char*> extensions;
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
    PickPhysicalDevice([](VkPhysicalDevice device) -> bool {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        VkQueueFamily graphicsQueueFamily;
        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                graphicsQueueFamily = i;
            }

            if (graphicsQueueFamily.has_value())
                break;

            i++;
        }

        return graphicsQueueFamily.has_value();
    });
    GetQueueFamilies();

    std::vector<const char*> deviceExtensions;
#ifdef HY_PLATFORM_APPLE
    deviceExtensions.push_back("VK_KHR_portability_subset");
#endif
    CreateLogicalDevice(deviceExtensions, {"VK_LAYER_KHRONOS_validation"});
    vkGetDeviceQueue(m_Device, m_GraphicsQueueFamily.value(), 0, &m_GraphicsQueue);

    HY_LOG_INFO("Created Vulkan context");
    HY_LOG_INFO("Using Vulkan API version 1.0");
}

VulkanContext::~VulkanContext() {
    vkDestroyDevice(m_Device, nullptr);

#ifdef HY_DEBUG
    Functions::DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
#endif

    vkDestroyInstance(m_Instance, nullptr);
}

void VulkanContext::CreateInstance(VkApplicationInfo appInfo, VkInstanceCreateFlags flags, const DynamicArray<const char*> extensions,
                                   const DynamicArray<const char*> validationLayers, PFN_vkDebugUtilsMessengerCallbackEXT debugCallback) {
    // Check for extensions
    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
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

    std::vector<VkLayerProperties> availableLayers(availableLayerCount);
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

void VulkanContext::PickPhysicalDevice(std::function<bool(VkPhysicalDevice)> deviceRateFunction) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
    HY_ASSERT(deviceCount > 0, "Failed to find physical device with vulkan support");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

    m_PhysicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties currentPhysicalDeviceProperties;
    size_t currentPhysicalDeviceMemorySize;

    for (const auto& device : devices) {
        if (!deviceRateFunction(device))
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

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            m_GraphicsQueueFamily = i;
        }

        if (m_GraphicsQueueFamily.has_value())
            break;

        i++;
    }

    HY_ASSERT(m_GraphicsQueueFamily.has_value(), "Graphics Family cant be found");
}

void VulkanContext::CreateLogicalDevice(const DynamicArray<const char*> extensions, const DynamicArray<const char*> validationLayers) {
    float graphicsQueuePriority = 1.0f;

    VkDeviceQueueCreateInfo graphicsQueueCreateInfo {};
    graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    graphicsQueueCreateInfo.queueFamilyIndex = m_GraphicsQueueFamily.value();
    graphicsQueueCreateInfo.queueCount = 1;
    graphicsQueueCreateInfo.pQueuePriorities = &graphicsQueuePriority;

    VkPhysicalDeviceFeatures deviceFeatures {};

    VkDeviceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &graphicsQueueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
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
