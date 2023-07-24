#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanVertexBuffer::VulkanVertexBuffer(const ReferencePointer<RenderDevice>& device, size_t size)
    : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(device)), m_Size(size) {
  ZoneScoped;

  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VK_CHECK_ERROR(vkCreateBuffer(m_RenderDevice->GetDevice(), &bufferInfo, nullptr, &m_VertexBuffer), "Failed to create vulkan vertex buffer!");

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(m_RenderDevice->GetDevice(), m_VertexBuffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  VK_CHECK_ERROR(vkAllocateMemory(m_RenderDevice->GetDevice(), &allocInfo, nullptr, &m_VertexBufferMemory), "Failed to allocate vulkan vertex buffer memory!");

  vkBindBufferMemory(m_RenderDevice->GetDevice(), m_VertexBuffer, m_VertexBufferMemory, 0);
}

VulkanVertexBuffer::VulkanVertexBuffer(const ReferencePointer<RenderDevice>& device, float* vertices, size_t size)
    : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(device)), m_Size(size) {
  ZoneScoped;

  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VK_CHECK_ERROR(vkCreateBuffer(m_RenderDevice->GetDevice(), &bufferInfo, nullptr, &m_VertexBuffer), "Failed to create vulkan vertex buffer!");

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(m_RenderDevice->GetDevice(), m_VertexBuffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  VK_CHECK_ERROR(vkAllocateMemory(m_RenderDevice->GetDevice(), &allocInfo, nullptr, &m_VertexBufferMemory), "Failed to allocate vulkan vertex buffer memory!");

  vkBindBufferMemory(m_RenderDevice->GetDevice(), m_VertexBuffer, m_VertexBufferMemory, 0);
  SetData(vertices, size);
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
  ZoneScoped;
  vkDestroyBuffer(m_RenderDevice->GetDevice(), m_VertexBuffer, nullptr);
  vkFreeMemory(m_RenderDevice->GetDevice(), m_VertexBufferMemory, nullptr);
}

void VulkanVertexBuffer::Bind() const { ZoneScoped; }

void VulkanVertexBuffer::Unbind() const { ZoneScoped; }

void VulkanVertexBuffer::SetData(const void* data, size_t size) {
  ZoneScoped;

  void* vertexData;
  vkMapMemory(m_RenderDevice->GetDevice(), m_VertexBufferMemory, 0, size, 0, &vertexData);
  memcpy(vertexData, data, (size_t)size);
  vkUnmapMemory(m_RenderDevice->GetDevice(), m_VertexBufferMemory);
}

uint32_t VulkanVertexBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(m_RenderDevice->GetPhysicalDevice(), &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }

  HY_INVOKE_ERROR("Failed to find vulkan suitable memory type for vulkan vertex buffer!");
}

VulkanIndexBuffer::VulkanIndexBuffer(const ReferencePointer<RenderDevice>& device, uint32_t* indices, size_t count)
    : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(device)) {
  ZoneScoped;
}

VulkanIndexBuffer::~VulkanIndexBuffer() { ZoneScoped; }

void VulkanIndexBuffer::Bind() const { ZoneScoped; }

void VulkanIndexBuffer::Unbind() const { ZoneScoped; }
