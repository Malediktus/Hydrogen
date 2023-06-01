#include <Hydrogen/Platform/Vulkan/VulkanBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size) {
    ZoneScoped;
    HY_LOG_TRACE("Created Vulkan vertex buffer of type dynamic (ID: {})", m_RendererID);
}

VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, uint32_t size) {
    ZoneScoped;
    HY_LOG_TRACE("Created Vulkan vertex buffer of type static (ID: {})", m_RendererID);
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
    ZoneScoped;
    HY_LOG_TRACE("Deleted Vulkan vertex buffer (ID: {})", m_RendererID);
}

void VulkanVertexBuffer::Bind() const {
    ZoneScoped;
    HY_LOG_TRACE("Bound Vulkan vertex buffer (ID: {})", m_RendererID);
}

void VulkanVertexBuffer::Unbind() const {
    ZoneScoped;
    HY_LOG_TRACE("Unbound Vulkan vertex buffer (ID: {})", m_RendererID);
}

void VulkanVertexBuffer::SetData(const void* data, uint32_t size) {
    ZoneScoped;
    HY_LOG_TRACE("Set data for Vulkan vertex buffer (ID: {})", m_RendererID);
}

VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count) {
    ZoneScoped;
    HY_LOG_TRACE("Created Vulkan index buffer of type static (ID: {})", m_RendererID);
}

VulkanIndexBuffer::~VulkanIndexBuffer() {
    ZoneScoped;
    HY_LOG_TRACE("Deleted Vulkan index buffer (ID: {})", m_RendererID);
}

void VulkanIndexBuffer::Bind() const {
    ZoneScoped;
    HY_LOG_TRACE("Bound Vulkan index buffer (ID: {})", m_RendererID);
}

void VulkanIndexBuffer::Unbind() const {
    ZoneScoped;
    HY_LOG_TRACE("Unbound Vulkan index buffer (ID: {})", m_RendererID);
}
