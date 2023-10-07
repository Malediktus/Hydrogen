#pragma once

#include <vulkan/vulkan.h>

#include "../../Renderer/Context.hpp"

namespace Hydrogen::Vulkan {
class VulkanContext : public Context {
 public:
  VulkanContext(const ReferencePointer<RenderWindow>& mainWindow);
  virtual ~VulkanContext();

  virtual void Init(ProjectInformation clientInfo, ProjectInformation engineInfo) override;

  const DynamicArray<char*>& GetInstanceExtensions() { return m_InstanceExtensions; }
  const DynamicArray<char*>& GetDeviceExtensions() { return m_DeviceExtensions; }
  const DynamicArray<char*>& GetValidationLayers() { return m_ValidationLayers; }

  VkInstance GetInstance() { return m_Instance; }

 private:
  void ConfigureExtensionsAndValidationLayers(const DynamicArray<char*>& requiredExtensions);
  void PopulateApplicationInfo(VkApplicationInfo& appInfo, const ProjectInformation& clientInfo, const ProjectInformation& engineInfo);
  void CreateInstance(VkApplicationInfo appInfo, VkInstanceCreateFlags flags, PFN_vkDebugUtilsMessengerCallbackEXT debugCallback);
  void CheckExtensionSupport(const DynamicArray<char*>& extensions);
  void CheckValidationLayerSupport(const DynamicArray<char*>& validationLayers);
  void CreateDebugMessenger(PFN_vkDebugUtilsMessengerCallbackEXT callback);
  void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo, PFN_vkDebugUtilsMessengerCallbackEXT callback);

  ReferencePointer<RenderWindow> m_MainWindow;
  VkInstance m_Instance;
  VkDebugUtilsMessengerEXT m_DebugMessenger;
  DynamicArray<char*> m_InstanceExtensions;
  DynamicArray<char*> m_DeviceExtensions;
  DynamicArray<char*> m_ValidationLayers;
};
}  // namespace Hydrogen::Vulkan
