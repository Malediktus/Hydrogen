#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanVertexArray.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanVertexArray::VulkanVertexArray() { ZoneScoped; }

VulkanVertexArray::~VulkanVertexArray() {
  ZoneScoped;
}

void VulkanVertexArray::Bind() const {
  ZoneScoped;
}

void VulkanVertexArray::Unbind() const {
  ZoneScoped;
}

void VulkanVertexArray::AddVertexBuffer(const ReferencePointer<VertexBuffer>& vertexBuffer) {
  ZoneScoped;
  HY_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "At lease one value is needed in vertex buffer layout");
}

void VulkanVertexArray::SetIndexBuffer(const ReferencePointer<IndexBuffer>& indexBuffer) {
  ZoneScoped;
  (void)indexBuffer;
}
