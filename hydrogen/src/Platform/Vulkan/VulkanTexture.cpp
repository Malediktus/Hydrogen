#include <Hydrogen/Core/Base.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanTexture.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

namespace Hydrogen::Vulkan::Utils {
uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }

  HY_INVOKE_ERROR("Failed to find suitable vulkan memory type!");
}
}  // namespace Hydrogen::Vulkan::Utils

VulkanTexture2D::VulkanTexture2D(const ReferencePointer<RenderWindow>& window, const uint32_t width, const uint32_t height, const void* data)
    : m_Width(width), m_Height(height), m_Window(window) {
  ZoneScoped;
  VkDeviceSize imageSize = width * height * 4;

  VulkanBuffer stagingBuffer(window, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  void* mappedMemory;
  vkMapMemory(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), stagingBuffer.GetBufferMemory(), 0, imageSize, 0, &mappedMemory);
  memcpy(mappedMemory, data, static_cast<size_t>(imageSize));
  vkUnmapMemory(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), stagingBuffer.GetBufferMemory());

  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = static_cast<uint32_t>(width);
  imageInfo.extent.height = static_cast<uint32_t>(height);
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.flags = 0;  // Optional

  VK_CHECK_ERROR(vkCreateImage(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &imageInfo, nullptr, &m_Image), "Failed to create vulkan image!");

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_Image, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      Utils::FindMemoryType(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetPhysicalDevice(), memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  VK_CHECK_ERROR(vkAllocateMemory(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &allocInfo, nullptr, &m_ImageMemory), "Failed to allocate vulkan memory!");

  vkBindImageMemory(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_Image, m_ImageMemory, 0);

  VkCommandBufferAllocateInfo commandBufferAllocInfo{};
  commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  commandBufferAllocInfo.commandPool = Renderer::GetRenderDevice<VulkanRenderDevice>()->GetCommandPool();
  commandBufferAllocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  VK_CHECK_ERROR(vkAllocateCommandBuffers(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &commandBufferAllocInfo, &commandBuffer),
                 "Failed to allocate vulkan command buffer!");

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  VK_CHECK_ERROR(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Failed to begin vulkan command buffer!");

  TransitionImageLayout(commandBuffer, m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

  VkBufferImageCopy region{};
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;

  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;

  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};

  vkCmdCopyBufferToImage(commandBuffer, stagingBuffer.GetBuffer(), m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

  TransitionImageLayout(commandBuffer, m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  VK_CHECK_ERROR(vkEndCommandBuffer(commandBuffer), "Failed to end vulkan command buffer!");

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  VK_CHECK_ERROR(vkQueueSubmit(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit vulkan graphics queue!");
  VK_CHECK_ERROR(vkQueueWaitIdle(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetGraphicsQueue()), "Failed to wait for vulkan graphics queue idle!");

  vkFreeCommandBuffers(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), Renderer::GetRenderDevice<VulkanRenderDevice>()->GetCommandPool(), 1, &commandBuffer);

  VkImageViewCreateInfo viewInfo{};
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = m_Image;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
  viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  VK_CHECK_ERROR(vkCreateImageView(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &viewInfo, nullptr, &m_ImageView), "Failed to create vulkan image view!");

  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.anisotropyEnable = VK_TRUE;

  VkPhysicalDeviceProperties properties{};
  vkGetPhysicalDeviceProperties(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetPhysicalDevice(), &properties);
  samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = 0.0f;

  VK_CHECK_ERROR(vkCreateSampler(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &samplerInfo, nullptr, &m_Sampler), "Failed to create vulkan sampler!");
}

VulkanTexture2D::~VulkanTexture2D() {
  ZoneScoped;

  vkDestroySampler(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_Sampler, nullptr);
  vkDestroyImageView(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_ImageView, nullptr);
  vkDestroyImage(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_Image, nullptr);
  vkFreeMemory(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_ImageMemory, nullptr);
}

void VulkanTexture2D::TransitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
  (void)format;

  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  VkPipelineStageFlags sourceStage;
  VkPipelineStageFlags destinationStage;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else {
    HY_INVOKE_ERROR("Unsupported vulkan layout transition!");
  }

  vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}
