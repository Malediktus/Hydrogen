#include <Hydrogen/Platform/Vulkan/VulkanVertexArray.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanVertexArray::VulkanVertexArray() {
    ZoneScoped;
}

VulkanVertexArray::~VulkanVertexArray() {
    ZoneScoped;
    HY_LOG_TRACE("Deleted Vulkan vertex array (ID: {})", m_RendererID);
}

void VulkanVertexArray::Bind() const {
    ZoneScoped;
    HY_LOG_TRACE("Bound Vulkan vertex array (ID: {})", m_RendererID);
}

void VulkanVertexArray::Unbind() const {
    ZoneScoped;
    HY_LOG_TRACE("Unbount Vulkan vertex array (ID: {})", m_RendererID);
}

void VulkanVertexArray::AddVertexBuffer(const ReferencePointer<VertexBuffer>& vertexBuffer) {
    ZoneScoped;
    HY_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "At lease one value is needed in vertex buffer layout");
    HY_LOG_TRACE("Attached vertex buffer to Vulkan vertex array (ID: {})", m_RendererID);
}

void VulkanVertexArray::SetIndexBuffer(const ReferencePointer<IndexBuffer>& indexBuffer) {
    ZoneScoped;
    HY_LOG_TRACE("Set index buffer of Vulkan vertex array (ID: {})", m_RendererID);
}
