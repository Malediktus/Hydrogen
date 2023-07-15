#pragma once

#include <vulkan/vulkan.h>

#include "../../Renderer/Context.hpp"

#define VK_CHECK_ERROR(expr, msg) HY_ASSERT((expr) == VK_SUCCESS, msg)

namespace Hydrogen::Vulkan {
class VulkanContext : public Context {
 public:
  VulkanContext(const ReferencePointer<RenderWindow>& window);
  virtual ~VulkanContext();

  virtual void Init(ProjectInformation clientInfo, ProjectInformation engineInfo) override;
  virtual ReferencePointer<RenderWindow> GetWindow() override { return m_Window; }

  const DynamicArray<const char*>& GetInstanceExtensions() { return m_InstanceExtensions; }
  const DynamicArray<const char*>& GetDeviceExtensions() { return m_DeviceExtensions; }
  const DynamicArray<const char*>& GetValidationLayers() { return m_ValidationLayers; }

  VkInstance GetInstance() { return m_Instance; }
  VkSurfaceKHR GetWindowSurface() { return m_WindowSurface; }

 private:
  void CreateInstance(VkApplicationInfo appInfo, VkInstanceCreateFlags flags, PFN_vkDebugUtilsMessengerCallbackEXT debugCallback);
  void CreateDebugMessenger(PFN_vkDebugUtilsMessengerCallbackEXT callback);
  void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo, PFN_vkDebugUtilsMessengerCallbackEXT callback);

  ReferencePointer<RenderWindow> m_Window;
  VkInstance m_Instance;
  VkDebugUtilsMessengerEXT m_DebugMessenger;
  VkSurfaceKHR m_WindowSurface;
  DynamicArray<const char*> m_InstanceExtensions;
  DynamicArray<const char*> m_DeviceExtensions;
  DynamicArray<const char*> m_ValidationLayers;
};
}  // namespace Hydrogen::Vulkan
