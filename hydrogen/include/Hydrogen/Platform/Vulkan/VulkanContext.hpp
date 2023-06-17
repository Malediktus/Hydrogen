#pragma once

#include "../../Renderer/Context.hpp"
#include <vulkan/vulkan.h>

#define VK_CHECK_ERROR(expr, msg) HY_ASSERT((expr) == VK_SUCCESS, msg)

using VkQueueFamily = std::optional<uint32_t>;

namespace Hydrogen::Vulkan {
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR Capabilities;
    DynamicArray<VkSurfaceFormatKHR> Formats;
    DynamicArray<VkPresentModeKHR> PresentModes;
};

class VulkanContext : public Context {
public:
    VulkanContext(const ReferencePointer<RenderWindow>& window);
    virtual ~VulkanContext();

    virtual void Init(ProjectInformation clientInfo, ProjectInformation engineInfo) override;
    virtual ReferencePointer<RenderWindow> GetWindow() override {
        return m_Window;
    }

    VkInstance GetInstance() {
        return m_Instance;
    }

    VkDevice GetDevice() {
        return m_Device;
    }

    VkPhysicalDevice GetPhysicalDevice() {
        return m_PhysicalDevice;
    }

    VkQueue GetGraphicsQueue() {
        return m_GraphicsQueue;
    }

    SwapChainSupportDetails GetSwapChainSupport(VkPhysicalDevice physicalDevice);

private:
    void CreateInstance(VkApplicationInfo appInfo, VkInstanceCreateFlags flags, const DynamicArray<const char*> extensions, const DynamicArray<const char*> validationLayers,
                        PFN_vkDebugUtilsMessengerCallbackEXT debugCallback);
    void CreateDebugMessenger(PFN_vkDebugUtilsMessengerCallbackEXT callback);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo, PFN_vkDebugUtilsMessengerCallbackEXT callback);
    void PickPhysicalDevice(
        DynamicArray<const char*> deviceExtensions, std::function<bool(VkPhysicalDevice, VkSurfaceKHR, VulkanContext*, DynamicArray<const char*>)> deviceRateFunction);
    void GetQueueFamilies();
    void CreateLogicalDevice(const DynamicArray<const char*> extensions, const DynamicArray<const char*> validationLayers);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void CreateSwapChain();

    ReferencePointer<RenderWindow> m_Window;
    VkInstance m_Instance;
    VkDebugUtilsMessengerEXT m_DebugMessenger;
    VkDevice m_Device;
    VkPhysicalDevice m_PhysicalDevice;
    VkQueueFamily m_GraphicsQueueFamily;
    VkQueueFamily m_PresentQueueFamily;
    VkQueue m_GraphicsQueue;
    VkQueue m_PresentQueue;
    VkSurfaceKHR m_WindowSurface;
    VkSwapchainKHR m_SwapChain;
};
} // namespace Hydrogen::Vulkan
