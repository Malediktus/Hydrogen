#pragma once

#include <vulkan/vulkan.h>

#include "../../Renderer/Buffer.hpp"

namespace Hydrogen::Vulkan {
class VulkanBuffer {
 public:
  VulkanBuffer(const ReferencePointer<class RenderWindow>& window, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
  virtual ~VulkanBuffer();

  VkBuffer GetBuffer() { return m_Buffer; }
  VkDeviceMemory GetBufferMemory() { return m_BufferMemory; }

 protected:
  ReferencePointer<class RenderWindow> m_Window;
  VkBuffer m_Buffer;
  VkDeviceMemory m_BufferMemory;
};

class VulkanVertexBuffer : public VertexBuffer, public VulkanBuffer {
 public:
  VulkanVertexBuffer(const ReferencePointer<class RenderWindow>& window, float* vertices, size_t size);
  virtual ~VulkanVertexBuffer();

  virtual void Bind() const override;

  virtual const BufferLayout& GetLayout() const override { return m_Layout; }
  virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

  VkBuffer GetVertexBuffer() { return m_Buffer; }
  size_t GetSize() { return m_Size; }

 private:
  BufferLayout m_Layout;
  size_t m_Size;
};

class VulkanIndexBuffer : public IndexBuffer, public VulkanBuffer {
 public:
  VulkanIndexBuffer(const ReferencePointer<class RenderWindow>& window, uint32_t* indices, size_t size);
  virtual ~VulkanIndexBuffer();

  virtual void Bind() const override;

  virtual size_t GetCount() const override { return m_Count; }

 private:
  size_t m_Count;
};

class VulkanUniformBuffer : public UniformBuffer, public VulkanBuffer {
 public:
  VulkanUniformBuffer(const ReferencePointer<class RenderWindow>& window, size_t size);
  virtual ~VulkanUniformBuffer();

  virtual void SetData(void* data) override;
  virtual size_t GetSize() { return m_Size; }

 private:
  size_t m_Size;
  void* m_MappedMemory;
};
}  // namespace Hydrogen::Vulkan
