#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanFramebuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;
using namespace Hydrogen::Vulkan;

VulkanFramebuffer::VulkanFramebuffer(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain,
                                     const ReferencePointer<RenderPass>& renderPass)
    : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(renderDevice)),
      m_SwapChain(std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)),
      m_RenderPass(std::dynamic_pointer_cast<VulkanRenderPass>(renderPass)) {
  ZoneScoped;
  auto swapChainImageViews = m_SwapChain->GetImageViews();
  auto swapChainExtent = m_SwapChain->GetExtent();

  m_SwapChainFramebuffers.resize(swapChainImageViews.size());

  for (size_t i = 0; i < swapChainImageViews.size(); i++) {
    VkImageView attachments[] = {swapChainImageViews[i]};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = m_RenderPass->GetRenderPass();
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = swapChainExtent.width;
    framebufferInfo.height = swapChainExtent.height;
    framebufferInfo.layers = 1;

    VK_CHECK_ERROR(vkCreateFramebuffer(m_RenderDevice->GetDevice(), &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]), "Failed to create vulkan framebuffer!");
  }
}

VulkanFramebuffer::~VulkanFramebuffer() {
  for (auto framebuffer : m_SwapChainFramebuffers) {
    vkDestroyFramebuffer(m_RenderDevice->GetDevice(), framebuffer, nullptr);
  }
}
