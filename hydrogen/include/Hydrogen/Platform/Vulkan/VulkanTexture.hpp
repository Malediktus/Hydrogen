#pragma once

#include <vulkan/vulkan.h>

#include "../../Renderer/Texture.hpp"

namespace Hydrogen::Vulkan {
class VulkanTexture2D : public Texture2D {
 public:
  VulkanTexture2D(const ReferencePointer<class RenderWindow>& window, const uint32_t width, const uint32_t height, const void* data);
  virtual ~VulkanTexture2D();

  virtual uint32_t GetWidth() const override { return m_Width; }
  virtual uint32_t GetHeight() const override { return m_Height; }

  VkImageView GetImageView() { return m_ImageView; }
  VkImage GetImage() { return m_Image; }
  VkSampler GetSampler() { return m_Sampler; }

 private:
  void TransitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

  uint32_t m_Width, m_Height;
  ReferencePointer<class RenderWindow> m_Window;

  VkImage m_Image;
  VkDeviceMemory m_ImageMemory;
  VkImageView m_ImageView;
  VkSampler m_Sampler;
};
}  // namespace Hydrogen::Vulkan
