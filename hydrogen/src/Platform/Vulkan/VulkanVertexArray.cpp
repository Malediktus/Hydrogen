#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanVertexArray.hpp>
#include <Hydrogen/Renderer/Buffer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanVertexArray::VulkanVertexArray() { ZoneScoped; }

VulkanVertexArray::~VulkanVertexArray() { ZoneScoped; }

void VulkanVertexArray::Bind() const {
  ZoneScoped;
  HY_ASSERT(m_IndexBuffer, "No index buffer provided!");
  HY_ASSERT(m_VertexBuffers.size() != 0, "No vertex buffers provided!");

  m_VertexBuffers[0]->Bind();
  m_IndexBuffer->Bind();
}

void VulkanVertexArray::AddVertexBuffer(const ReferencePointer<VertexBuffer>& vertexBuffer) {
  ZoneScoped;
  HY_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "At lease one value is needed in vertex buffer layout");
  HY_ASSERT(m_VertexBuffers.size() == 0, "Multiple vertex buffers per vertex array are unimplemented!");

  m_VertexBuffers.push_back(vertexBuffer);
}

void VulkanVertexArray::SetIndexBuffer(const ReferencePointer<IndexBuffer>& indexBuffer) {
  ZoneScoped;

  m_IndexBuffer = indexBuffer;
}
