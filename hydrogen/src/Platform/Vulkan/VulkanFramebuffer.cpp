#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanFramebuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;
using namespace Hydrogen::Vulkan;

VulkanFramebuffer::VulkanFramebuffer(const ReferencePointer<RenderWindow>&) {
  ZoneScoped;
  HY_LOG_TRACE("Created Vulkan framebuffer (ID: {})", m_RendererID);
}

VulkanFramebuffer::~VulkanFramebuffer() {
  ZoneScoped;
  HY_LOG_TRACE("Deleted Vulkan framebuffer (ID: {})", m_RendererID);
}

void VulkanFramebuffer::Bind() const {
  ZoneScoped;
  HY_LOG_TRACE("Bound Vulkan framebuffer (ID: {})", m_RendererID);
}

void VulkanFramebuffer::BindWrite() const {
  ZoneScoped;
  HY_LOG_TRACE("Bound Vulkan framebuffer for drawing (ID: {})", m_RendererID);
}

void VulkanFramebuffer::BindRead() const {
  ZoneScoped;
  HY_LOG_TRACE("Bound Vulkan framebuffer for reading (ID: {})", m_RendererID);
}

void VulkanFramebuffer::Unbind() const {
  ZoneScoped;
  HY_LOG_TRACE("Unbound Vulkan framebuffer (ID: {})", m_RendererID);
}

void VulkanFramebuffer::SetDrawBuffers(
    const DynamicArray<std::pair<FramebufferAttachment, uint32_t>>&
        attachments) {
  ZoneScoped;
  HY_LOG_TRACE("Set Vulkan draw buffers for framebuffer (ID: {})",
               m_RendererID);
}

void VulkanFramebuffer::AttachColorTexture(
    const ReferencePointer<Texture2D>& texture) {
  ZoneScoped;
  HY_ASSERT(numColorAttachments < 32, "Too many color attachments");
  HY_LOG_TRACE(
      "Attached texture of type color buffer to Vulkan framebuffer (ID: {}, "
      "TextureID: {})",
      0, 0);
}

void VulkanFramebuffer::AttachDepthTexture(
    const ReferencePointer<Texture2D>& texture) {
  ZoneScoped;
  HY_ASSERT(numDepthAttachments < 1 && numDepthStencilAttachments < 1,
            "Too many depth attachments");
  HY_LOG_TRACE(
      "Attached texture of type depth buffer to Vulkan framebuffer (ID: {}, "
      "TextureID: {})",
      0, 0);
}

void VulkanFramebuffer::AttachStencilTexture(
    const ReferencePointer<Texture2D>& texture) {
  ZoneScoped;
  HY_ASSERT(numStencilAttachments < 1 && numDepthStencilAttachments < 1,
            "Too many stencil attachments");
  HY_LOG_TRACE(
      "Attached texture of type stencil buffer to Vulkan framebuffer (ID: {}, "
      "TextureID: {})",
      0, 0);
}

void VulkanFramebuffer::AttachDepthStencilTexture(
    const ReferencePointer<Texture2D>& texture) {
  ZoneScoped;
  HY_ASSERT(numDepthStencilAttachments < 1 && numDepthAttachments < 1 &&
                numStencilAttachments < 1,
            "Too many depth/stencil attachments");
  HY_LOG_TRACE(
      "Attached texture of type depth_stencil buffer to Vulkan framebuffer "
      "(ID: {}, TextureID: {})",
      0, 0);
}
