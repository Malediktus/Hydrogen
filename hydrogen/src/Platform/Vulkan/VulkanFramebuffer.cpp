#include <Hydrogen/Platform/Vulkan/VulkanFramebuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanSwapChain.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanCommandBuffer.hpp>
#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Core/Base.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;
using namespace Hydrogen::Vulkan;

VulkanFramebuffer::VulkanFramebuffer(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain)
    : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(renderDevice)),
      m_SwapChain(std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)),
      m_ClearColor(0.0f, 0.0f, 0.0f, 1.0f) {
  ZoneScoped;

  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = m_SwapChain->GetSwapChainImageFormat();
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  VK_CHECK_ERROR(vkCreateRenderPass(m_RenderDevice->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass), "Failed to create vulkan render pass!");

  const auto& swapChainImageViews = m_SwapChain->GetImageViews();
  const auto& swapChainExtent = m_SwapChain->GetExtent();

  m_Framebuffers.resize(swapChainImageViews.size());

  for (size_t i = 0; i < swapChainImageViews.size(); i++) {
    VkImageView attachments[] = {swapChainImageViews[i]};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = m_RenderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = swapChainExtent.width;
    framebufferInfo.height = swapChainExtent.height;
    framebufferInfo.layers = 1;

    VK_CHECK_ERROR(vkCreateFramebuffer(m_RenderDevice->GetDevice(), &framebufferInfo, nullptr, &m_Framebuffers[i]), "Failed to create vulkan framebuffer!");
  }
}

VulkanFramebuffer::~VulkanFramebuffer() {
  ZoneScoped;
  for (auto framebuffer : m_Framebuffers) {
    vkDestroyFramebuffer(m_RenderDevice->GetDevice(), framebuffer, nullptr);
  }
  vkDestroyRenderPass(m_RenderDevice->GetDevice(), m_RenderPass, nullptr);
}

void VulkanFramebuffer::Bind(const ReferencePointer<CommandBuffer>& commandBuffer) {
  ZoneScoped;

  auto vulkanCommandBuffer = std::dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer);

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = m_RenderPass;
  renderPassInfo.framebuffer = m_Framebuffers[vulkanCommandBuffer->GetImageIndex()];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = m_SwapChain->GetExtent();

  VkClearValue clearColor = {{{m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(vulkanCommandBuffer->GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}
