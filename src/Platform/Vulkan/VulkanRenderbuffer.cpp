#include <Hydrogen/Platform/Vulkan/VulkanRenderbuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanRenderbuffer::VulkanRenderbuffer(uint32_t width, uint32_t height, RenderbufferUsageType usageType) {
    ZoneScoped;
    HY_LOG_TRACE("Created Vulkan renderbuffer (ID: {})", m_RendererID);
}

VulkanRenderbuffer::~VulkanRenderbuffer() {
    ZoneScoped;
    HY_LOG_TRACE("Deleted Vulkan renderbuffer (ID: {})", m_RendererID);
}

void VulkanRenderbuffer::Bind() const {
    ZoneScoped;
    HY_LOG_TRACE("Bound Vulkan renderbuffer (ID: {})", m_RendererID);
}

void VulkanRenderbuffer::Unbind() const {
    ZoneScoped;
    HY_LOG_TRACE("Unbound Vulkan renderbuffer (ID: {})", m_RendererID);
}

void VulkanRenderbuffer::Resize(uint32_t width, uint32_t height) {
    ZoneScoped;
    HY_LOG_TRACE("Resized Vulkan renderbuffer (ID: {})", m_RendererID);
}

void* VulkanRenderbuffer::GetNative() const {
}
