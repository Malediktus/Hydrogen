#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Core/Base.hpp>
#include <Hydrogen/Core/Window.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <set>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

namespace Hydrogen::Vulkan::Utils {
static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                          const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void*) {
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
}  // namespace Hydrogen::Vulkan::Utils

namespace Hydrogen::Vulkan::Functions {
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}
}  // namespace Hydrogen::Vulkan::Functions

VulkanContext::VulkanContext(const ReferencePointer<RenderWindow>& mainWindow) : m_MainWindow(mainWindow) {}

void VulkanContext::Init(ProjectInformation clientInfo, ProjectInformation engineInfo) {
  ZoneScoped;

  auto windowExtensions = m_MainWindow->GetVulkanWindowExtensions();
  ConfigureExtensionsAndValidationLayers(windowExtensions);
  VkApplicationInfo appInfo{};
  PopulateApplicationInfo(appInfo, clientInfo, engineInfo);
  VkInstanceCreateFlags instanceFlags = 0;
  CheckExtensionSupport(m_InstanceExtensions);
#ifdef HY_DEBUG
  CheckValidationLayerSupport(m_ValidationLayers);
#endif
#ifdef HY_PLATFORM_APPLE
  instanceFlags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
  CreateInstance(appInfo, instanceFlags, Utils::VulkanDebugCallback);
  CreateDebugMessenger(Utils::VulkanDebugCallback);

  HY_LOG_INFO("Created Vulkan context");
  HY_LOG_INFO("Using Vulkan API version 1.0");
}

VulkanContext::~VulkanContext() {
#ifdef HY_DEBUG
  Functions::DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
#endif

  vkDestroyInstance(m_Instance, nullptr);
}

void VulkanContext::ConfigureExtensionsAndValidationLayers(const DynamicArray<const char*>& requiredExtensions) {
  m_ValidationLayers.push_back((char*)"VK_LAYER_KHRONOS_validation");
#ifdef HY_DEBUG
  m_InstanceExtensions.push_back((char*)VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
#ifdef HY_PLATFORM_APPLE
  m_InstanceExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  m_InstanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
  m_DeviceExtensions.push_back("VK_KHR_portability_subset");
#endif
  m_InstanceExtensions.insert(m_InstanceExtensions.end(), requiredExtensions.begin(), requiredExtensions.end());
  m_DeviceExtensions.push_back((char*)VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void VulkanContext::PopulateApplicationInfo(VkApplicationInfo& appInfo, const ProjectInformation& clientInfo, const ProjectInformation& engineInfo) {
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pNext = nullptr;
  appInfo.pApplicationName = clientInfo.ProjectName.c_str();
  appInfo.applicationVersion = VK_MAKE_VERSION(clientInfo.ProjectVersion.x, clientInfo.ProjectVersion.y, clientInfo.ProjectVersion.z);
  appInfo.pEngineName = engineInfo.ProjectName.c_str();
  appInfo.engineVersion = VK_MAKE_VERSION(engineInfo.ProjectVersion.x, engineInfo.ProjectVersion.y, engineInfo.ProjectVersion.z);
  appInfo.apiVersion = VK_API_VERSION_1_0;
}

void VulkanContext::CreateInstance(VkApplicationInfo appInfo, VkInstanceCreateFlags flags, PFN_vkDebugUtilsMessengerCallbackEXT debugCallback) {
  VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
  PopulateDebugMessengerCreateInfo(debugMessengerCreateInfo, debugCallback);

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pNext = nullptr;
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;
#ifdef HY_DEBUG
  createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerCreateInfo;
  createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
  createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
#endif
  createInfo.pApplicationInfo = &appInfo;
  createInfo.flags = flags;
  createInfo.enabledExtensionCount = static_cast<uint32_t>(m_InstanceExtensions.size());
  createInfo.ppEnabledExtensionNames = m_InstanceExtensions.data();

  VK_CHECK_ERROR(vkCreateInstance(&createInfo, nullptr, &m_Instance), "Failed to create vulkan instance!");
}

void VulkanContext::CheckExtensionSupport(const DynamicArray<const char*>& extensions) {
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

    if (found) continue;

    HY_LOG_ERROR("Vulkan extension '{}' is not supported!", extension);
    HY_INVOKE_ERROR("A vulkan extension is not supported");
  }
}

void VulkanContext::CheckValidationLayerSupport(const DynamicArray<const char*>& validationLayers) {
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

    if (found) continue;

    HY_LOG_ERROR("Vulkan validation layer '{}' is not supported!", validationLayer);
    HY_INVOKE_ERROR("A vulkan validation layer is not supported");
  }
}

void VulkanContext::CreateDebugMessenger(PFN_vkDebugUtilsMessengerCallbackEXT callback) {
#ifdef HY_DEBUG
  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  PopulateDebugMessengerCreateInfo(createInfo, callback);
  VK_CHECK_ERROR(Functions::CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger), "Failed to create vulkan debug messenger!");
#endif
}

void VulkanContext::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo, PFN_vkDebugUtilsMessengerCallbackEXT callback) {
#ifndef HY_DEBUG
  return;
#endif
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = callback;
  createInfo.pUserData = nullptr;
}
