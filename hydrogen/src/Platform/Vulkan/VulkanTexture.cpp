#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanTexture.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanTexture2D::VulkanTexture2D(const int width, const int height,
                                 Texture2DStorageType storageType) {
  ZoneScoped;
  HY_LOG_TRACE("Created Vulkan texture2D from file (ID: {})", m_RendererID);
}

VulkanTexture2D::VulkanTexture2D(const int width, const int height,
                                 const void* data,
                                 Texture2DStorageType storageType) {
  ZoneScoped;
  HY_LOG_TRACE("Created Vulkan texture2D from file (ID: {})", m_RendererID);
}

VulkanTexture2D::~VulkanTexture2D() {
  ZoneScoped;
  HY_LOG_TRACE("Deleted Vulkan texture2D (ID: {})", m_RendererID);
}

void VulkanTexture2D::Bind(uint32_t slot) const {
  ZoneScoped;
  HY_LOG_TRACE("Bound Vulkan texture2D (ID: {})", m_RendererID);
}

void VulkanTexture2D::Resize(uint32_t width, uint32_t height) {
  ZoneScoped;
  HY_LOG_TRACE("Resized Vulkan texture2D (ID: {})", m_RendererID);
}

void* VulkanTexture2D::GetNative() const {}
