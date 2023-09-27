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

VulkanBuffer::VulkanBuffer(ReferencePointer<VulkanRenderDevice> renderDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    : m_RenderDevice(renderDevice) {
  ZoneScoped;

  auto device = renderDevice->GetDevice();

  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VK_CHECK_ERROR(vkCreateBuffer(device, &bufferInfo, nullptr, &m_Buffer), "Failed to create vulkan buffer");

  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(device, m_Buffer, &memoryRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memoryRequirements.size;
  allocInfo.memoryTypeIndex = Utils::FindMemoryType(memoryRequirements.memoryTypeBits, properties, renderDevice->GetPhysicalDevice());

  VK_CHECK_ERROR(vkAllocateMemory(device, &allocInfo, nullptr, &m_BufferMemory), "Failed to allocate buffer memory");

  vkBindBufferMemory(device, m_Buffer, m_BufferMemory, 0);
}

VulkanBuffer::~VulkanBuffer() {
  vkDestroyBuffer(m_RenderDevice->GetDevice(), m_Buffer, nullptr);
  vkFreeMemory(m_RenderDevice->GetDevice(), m_BufferMemory, nullptr);
}

VulkanVertexBuffer::VulkanVertexBuffer(const ReferencePointer<RenderDevice>& device, float* vertices, size_t size)
    : m_Size(size),
      VulkanBuffer(std::dynamic_pointer_cast<VulkanRenderDevice>(device), size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
  ZoneScoped;

  auto vulkanDevice = m_RenderDevice->GetDevice();

  VulkanBuffer stagingBuffer(std::dynamic_pointer_cast<VulkanRenderDevice>(device), size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  void* data;
  vkMapMemory(vulkanDevice, stagingBuffer.GetBufferMemory(), 0, size, 0, &data);
  memcpy(data, vertices, size);
  vkUnmapMemory(vulkanDevice, stagingBuffer.GetBufferMemory());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = m_RenderDevice->GetCommandPool();
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(vulkanDevice, &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);
  VkBufferCopy copyRegion{};
  copyRegion.srcOffset = 0;
  copyRegion.dstOffset = 0;
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, stagingBuffer.GetBuffer(), m_Buffer, 1, &copyRegion);
  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(m_RenderDevice->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(m_RenderDevice->GetGraphicsQueue());

  vkFreeCommandBuffers(vulkanDevice, m_RenderDevice->GetCommandPool(), 1, &commandBuffer);
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
}

void VulkanVertexBuffer::Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const {
  ZoneScoped;
  VkBuffer vertexBuffers[] = {m_Buffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(std::dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer(), 0, 1, vertexBuffers, offsets);
}

VulkanIndexBuffer::VulkanIndexBuffer(const ReferencePointer<RenderDevice>& device, uint32_t* indices, size_t size)
    : m_Count(size / sizeof(uint32_t)),
      VulkanBuffer(std::dynamic_pointer_cast<VulkanRenderDevice>(device), size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
  ZoneScoped;

  auto vulkanDevice = m_RenderDevice->GetDevice();

  VulkanBuffer stagingBuffer(std::dynamic_pointer_cast<VulkanRenderDevice>(device), size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  void* data;
  vkMapMemory(vulkanDevice, stagingBuffer.GetBufferMemory(), 0, size, 0, &data);
  memcpy(data, indices, size);
  vkUnmapMemory(vulkanDevice, stagingBuffer.GetBufferMemory());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = m_RenderDevice->GetCommandPool();
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(vulkanDevice, &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);
  VkBufferCopy copyRegion{};
  copyRegion.srcOffset = 0;
  copyRegion.dstOffset = 0;
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, stagingBuffer.GetBuffer(), m_Buffer, 1, &copyRegion);
  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(m_RenderDevice->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(m_RenderDevice->GetGraphicsQueue());

  vkFreeCommandBuffers(vulkanDevice, m_RenderDevice->GetCommandPool(), 1, &commandBuffer);
}

VulkanIndexBuffer::~VulkanIndexBuffer() { ZoneScoped; }

void VulkanIndexBuffer::Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const {
  ZoneScoped;
  vkCmdBindIndexBuffer(std::dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer(), m_Buffer, 0, VK_INDEX_TYPE_UINT32);
}

VulkanUniformBuffer::VulkanUniformBuffer(const ReferencePointer<RenderDevice>& device, size_t size)
    : VulkanBuffer(std::dynamic_pointer_cast<VulkanRenderDevice>(device), size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
      m_Size(size) {
  vkMapMemory(m_RenderDevice->GetDevice(), m_BufferMemory, 0, size, 0, &m_MappedMemory);
}

VulkanUniformBuffer::~VulkanUniformBuffer() {}

void VulkanUniformBuffer::SetData(void* data) { memcpy(m_MappedMemory, data, m_Size); }
