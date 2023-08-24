#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanCommandBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

namespace Hydrogen::Vulkan::Utils {
static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
  VkPhysicalDeviceMemoryProperties memoryProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

  for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }

  HY_INVOKE_ERROR("Failed to find suitable memory type!");
}
}

VulkanBuffer::VulkanBuffer(ReferencePointer<VulkanRenderDevice> renderDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
  ZoneScoped;

  auto device = renderDevice->GetDevice();

  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VK_CHECK_ERROR(vkCreateBuffer(device, &bufferInfo, nullptr, &m_Buffer));

  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(device, m_Buffer, &memoryRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memoryRequirements.size;
  allocInfo.memoryTypeIndex = Utils::FindMemoryType(memoryRequirements.memoryTypeBits, properties, renderDevice->GetPhysicalDevice());

  VK_CHECK_ERROR(vkAllocateMemory(device, &allocInfo, nullptr, &m_BufferMemory));

  vkBindBufferMemory(device, m_Buffer, m_BufferMemory, 0);
}

VulkanVertexBuffer::VulkanVertexBuffer(const ReferencePointer<RenderDevice>& device, size_t size)
    : VulkanBuffer(std::dynamic_pointer_cast<VulkanRenderDevice>(device), size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
    m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(device)), m_Size(size) {
  ZoneScoped;
}

VulkanVertexBuffer::VulkanVertexBuffer(const ReferencePointer<RenderDevice>& device, float* vertices, size_t size)
    : VulkanBuffer(std::dynamic_pointer_cast<VulkanRenderDevice>(device), size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
      m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(device)), m_Size(size) {
  ZoneScoped;

  SetData(vertices, size);
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
  ZoneScoped;
  vkDestroyBuffer(m_RenderDevice->GetDevice(), m_Buffer, nullptr);
  vkFreeMemory(m_RenderDevice->GetDevice(), m_BufferMemory, nullptr);
}

void VulkanVertexBuffer::Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const {
  ZoneScoped;
  VkBuffer vertexBuffers[] = {m_Buffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(std::dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer(), 0, 1, vertexBuffers, offsets);
}

void VulkanVertexBuffer::SetData(const void* data, size_t size) {
  ZoneScoped;

  void* vertexData;
  vkMapMemory(m_RenderDevice->GetDevice(), m_BufferMemory, 0, size, 0, &vertexData);
  memcpy(vertexData, data, (size_t)size);
  vkUnmapMemory(m_RenderDevice->GetDevice(), m_BufferMemory);
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
  (void)indices;
  (void)count;
}

VulkanIndexBuffer::~VulkanIndexBuffer() { ZoneScoped; }

void VulkanIndexBuffer::Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const {
  ZoneScoped;
  (void)commandBuffer;
}
