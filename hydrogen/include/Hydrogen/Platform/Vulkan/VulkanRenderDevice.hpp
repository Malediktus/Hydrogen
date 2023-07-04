#include "../../Renderer/RenderDevice.hpp"
#include "VulkanContext.hpp"

namespace Hydrogen::Vulkan {
using VkQueueFamily = std::optional<uint32_t>;

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR Capabilities;
    DynamicArray<VkSurfaceFormatKHR> Formats;
    DynamicArray<VkPresentModeKHR> PresentModes;
};

class VulkanRenderDevice : public RenderDevice {
public:
    VulkanRenderDevice(std::function<std::size_t(const RenderDeviceProperties&)> deviceRateFunction);
    virtual ~VulkanRenderDevice();

private:
    VkPhysicalDevice m_PhysicalDevice;
};
} // namespace Hydrogen::Vulkan
