#pragma once

#include "../../Renderer/VertexArray.hpp"

namespace Hydrogen::Vulkan {
class VulkanVertexArray : public VertexArray {
 public:
  VulkanVertexArray();
  virtual ~VulkanVertexArray();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual void AddVertexBuffer(
      const ReferencePointer<VertexBuffer>& vertexBuffer) override;
  virtual void SetIndexBuffer(
      const ReferencePointer<IndexBuffer>& indexBuffer) override;

  virtual void SetRenderPrimitive(RenderPrimitive primitive) override {
    m_Primitive = primitive;
  }

  virtual RenderPrimitive GetRenderPrimitive() const override {
    return m_Primitive;
  }

  virtual const DynamicArray<ReferencePointer<VertexBuffer>>& GetVertexBuffers()
      const override {
    return m_VertexBuffers;
  }
  virtual const ReferencePointer<IndexBuffer>& GetIndexBuffer() const override {
    return m_IndexBuffer;
  }

 private:
  uint32_t m_RendererID;
  RenderPrimitive m_Primitive;
  DynamicArray<ReferencePointer<VertexBuffer>> m_VertexBuffers;
  ReferencePointer<IndexBuffer> m_IndexBuffer;
};
}  // namespace Hydrogen::Vulkan
