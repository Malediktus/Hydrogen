#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

void VulkanContext::Init() {
    ZoneScoped;
    HY_LOG_INFO("Created Vulkan context");
    HY_LOG_INFO("Using Vulkan version {}.{}", 0, 0);
}
