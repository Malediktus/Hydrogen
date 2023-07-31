#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanTexture.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanTexture2D::VulkanTexture2D(const int width, const int height, Texture2DStorageType storageType) {
  ZoneScoped;
  (void)width;
  (void)height;
  (void)storageType;
}

VulkanTexture2D::VulkanTexture2D(const int width, const int height, const void* data, Texture2DStorageType storageType) {
  ZoneScoped;
  (void)width;
  (void)height;
  (void)data;
  (void)storageType;
}

VulkanTexture2D::~VulkanTexture2D() {
  ZoneScoped;
}

void VulkanTexture2D::Bind(uint32_t slot) const {
  ZoneScoped;
  (void)slot;
}

void VulkanTexture2D::Resize(uint32_t width, uint32_t height) {
  ZoneScoped;
  (void)width;
  (void)height;
}

void* VulkanTexture2D::GetNative() const { return nullptr; }
