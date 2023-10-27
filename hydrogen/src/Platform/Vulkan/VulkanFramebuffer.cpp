#include <Hydrogen/Platform/Vulkan/VulkanFramebuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanSwapChain.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanCommandBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanSwapChain.hpp>
#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Core/Base.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;
using namespace Hydrogen::Vulkan;

VulkanFramebuffer::VulkanFramebuffer(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain)
    : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(renderDevice)),
      m_SwapChain(std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)) {
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

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = VulkanSwapChain::FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL,
                                                                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, m_RenderDevice->GetPhysicalDevice());
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = 1;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcAccessMask = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  StaticArray<VkAttachmentDescription, 2> renderPassAttachments = {colorAttachment, depthAttachment};
  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = static_cast<uint32_t>(renderPassAttachments.size());
  renderPassInfo.pAttachments = renderPassAttachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  VK_CHECK_ERROR(vkCreateRenderPass(m_RenderDevice->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass), "Failed to create vulkan render pass!");

  const auto& swapChainImageViews = m_SwapChain->GetImageViews();
  const auto& swapChainExtent = m_SwapChain->GetExtent();

  m_Framebuffers.resize(swapChainImageViews.size());

  for (size_t i = 0; i < swapChainImageViews.size(); i++) {
    StaticArray<VkImageView, 2> attachments = {swapChainImageViews[i], m_SwapChain->GetDepthImageView()};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = m_RenderPass;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
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

void VulkanFramebuffer::Begin(Vector4 clearColor, const ReferencePointer<class CommandBuffer>& commandBuffer) {
  ZoneScoped;

  auto vulkanCommandBuffer = std::dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer);

  StaticArray<VkClearValue, 2> clearValues{};
  clearValues[0].color = {{clearColor.r, clearColor.g, clearColor.b, clearColor.a}};
  clearValues[1].depthStencil = {1.0f, 0};

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = m_RenderPass;
  renderPassInfo.framebuffer = m_Framebuffers[vulkanCommandBuffer->GetImageIndex()];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = m_SwapChain->GetExtent();
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(vulkanCommandBuffer->GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanFramebuffer::End(const ReferencePointer<class CommandBuffer>& commandBuffer) {
  ZoneScoped;
  vkCmdEndRenderPass(std::dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer());
}
