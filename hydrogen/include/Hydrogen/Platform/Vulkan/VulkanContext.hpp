#pragma once

#include "../../Renderer/Context.hpp"
#include <vulkan/vulkan.h>

#define VK_CHECK_ERROR(expr, msg) HY_ASSERT((expr) == VK_SUCCESS, msg)

namespace Hydrogen::Vulkan {
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

private:
    void CreateInstance(VkApplicationInfo appInfo, VkInstanceCreateFlags flags, const DynamicArray<const char*> extensions, const DynamicArray<const char*> validationLayers,
                        PFN_vkDebugUtilsMessengerCallbackEXT debugCallback);
    void CreateDebugMessenger(PFN_vkDebugUtilsMessengerCallbackEXT callback);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo, PFN_vkDebugUtilsMessengerCallbackEXT callback);

    ReferencePointer<RenderWindow> m_Window;
    VkInstance m_Instance;
    VkDebugUtilsMessengerEXT m_DebugMessenger;
};
} // namespace Hydrogen::Vulkan
