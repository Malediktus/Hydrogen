#pragma once

#include "../../Renderer/Buffer.hpp"
#include "VulkanRenderDevice.hpp"

namespace Hydrogen::Vulkan {
class VulkanVertexBuffer : public VertexBuffer {
 public:
  VulkanVertexBuffer(const ReferencePointer<RenderDevice>& device, size_t size);
  VulkanVertexBuffer(const ReferencePointer<RenderDevice>& device, float* vertices, size_t size);
  virtual ~VulkanVertexBuffer();

  virtual void Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const override;

  virtual void SetData(const void* data, size_t size) override;

  virtual const BufferLayout& GetLayout() const override { return m_Layout; }
  virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

  VkBuffer GetVertexBuffer() { return m_VertexBuffer; }
  size_t GetSize() { return m_Size; }

 private:
  uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  VkBuffer m_VertexBuffer;
  VkDeviceMemory m_VertexBufferMemory;
  BufferLayout m_Layout;
  size_t m_Size;
};

class VulkanIndexBuffer : public IndexBuffer {
 public:
  VulkanIndexBuffer(const ReferencePointer<RenderDevice>& device, uint32_t* indices, size_t count);
  virtual ~VulkanIndexBuffer();

  virtual void Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const override;

  virtual uint32_t GetCount() const override { return m_Count; }

 private:
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  uint32_t m_Count;
};
}  // namespace Hydrogen::Vulkan
