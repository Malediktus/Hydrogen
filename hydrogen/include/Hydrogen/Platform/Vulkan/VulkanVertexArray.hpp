#pragma once

#include "../../Renderer/VertexArray.hpp"

namespace Hydrogen::Vulkan {
class VulkanVertexArray : public VertexArray {
 public:
  VulkanVertexArray();
  virtual ~VulkanVertexArray();

  virtual void Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const override;

  virtual void AddVertexBuffer(const ReferencePointer<VertexBuffer>& vertexBuffer) override;
  virtual void SetIndexBuffer(const ReferencePointer<IndexBuffer>& indexBuffer) override;

  virtual const DynamicArray<ReferencePointer<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
  virtual const ReferencePointer<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

 private:
  DynamicArray<ReferencePointer<VertexBuffer>> m_VertexBuffers;
  ReferencePointer<IndexBuffer> m_IndexBuffer;
};
}  // namespace Hydrogen::Vulkan
