#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanCommandBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;
using namespace Hydrogen::Vulkan;

VulkanCommandBuffer::VulkanCommandBuffer(const ReferencePointer<RenderDevice>& renderDevice) : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(renderDevice)) {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = m_RenderDevice->GetCommandPool();
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = 1;

  VK_CHECK_ERROR(vkAllocateCommandBuffers(m_RenderDevice->GetDevice(), &allocInfo, &m_CommandBuffer), "Failed to allocate vulkan command buffer!");

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VK_CHECK_ERROR(vkCreateSemaphore(m_RenderDevice->GetDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphore), "Failed to create vulkan semaphore!");
  VK_CHECK_ERROR(vkCreateSemaphore(m_RenderDevice->GetDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphore), "Failed to create vulkan semaphore!");

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

  VK_CHECK_ERROR(vkCreateFence(m_RenderDevice->GetDevice(), &fenceInfo, nullptr, &m_InFlightFence), "Failed to create vulkan fence!");
  vkResetFences(m_RenderDevice->GetDevice(), 1, &m_InFlightFence);
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
  vkWaitForFences(m_RenderDevice->GetDevice(), 1, &m_InFlightFence, VK_TRUE, UINT64_MAX);
  vkResetFences(m_RenderDevice->GetDevice(), 1, &m_InFlightFence);

  vkDestroySemaphore(m_RenderDevice->GetDevice(), m_ImageAvailableSemaphore, nullptr);
  vkDestroySemaphore(m_RenderDevice->GetDevice(), m_RenderFinishedSemaphore, nullptr);
  vkDestroyFence(m_RenderDevice->GetDevice(), m_InFlightFence, nullptr);
}

void VulkanCommandBuffer::Reset() {
  vkWaitForFences(m_RenderDevice->GetDevice(), 1, &m_InFlightFence, VK_TRUE, UINT64_MAX);
  vkResetFences(m_RenderDevice->GetDevice(), 1, &m_InFlightFence);

  vkResetCommandBuffer(m_CommandBuffer, 0);
}

void VulkanCommandBuffer::Begin(const ReferencePointer<RenderPass>& renderPass, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Framebuffer>& framebuffer,
                                Vector4 clearColor) {
  vkAcquireNextImageKHR(m_RenderDevice->GetDevice(), std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)->GetSwapChain(), UINT64_MAX, m_ImageAvailableSemaphore, VK_NULL_HANDLE,
                        &m_ImageIndex);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;                   // Optional
  beginInfo.pInheritanceInfo = nullptr;  // Optional

  VK_CHECK_ERROR(vkBeginCommandBuffer(m_CommandBuffer, &beginInfo), "Failed to begin vulkan command buffer!");

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = std::dynamic_pointer_cast<VulkanRenderPass>(renderPass)->GetRenderPass();
  renderPassInfo.framebuffer = std::dynamic_pointer_cast<VulkanFramebuffer>(framebuffer)->GetFramebuffers()[m_ImageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)->GetExtent();

  VkClearValue vkClearColor = {{{clearColor.r, clearColor.g, clearColor.b, clearColor.a}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &vkClearColor;

  vkCmdBeginRenderPass(m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandBuffer::End(const ReferencePointer<SwapChain> swapChain) {
  vkCmdEndRenderPass(m_CommandBuffer);
  VK_CHECK_ERROR(vkEndCommandBuffer(m_CommandBuffer), "Failed to end vulkan command buffer!");

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores1[] = {m_ImageAvailableSemaphore};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores1;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &m_CommandBuffer;

  VkSemaphore signalSemaphores[] = {m_RenderFinishedSemaphore};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  VK_CHECK_ERROR(vkQueueSubmit(m_RenderDevice->GetGraphicsQueue(), 1, &submitInfo, m_InFlightFence), "Failed to submit vulkan graphics queue!");

  VkSemaphore waitSemaphores2[] = {m_RenderFinishedSemaphore};

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = waitSemaphores2;

  VkSwapchainKHR swapChains[] = {std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)->GetSwapChain()};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &m_ImageIndex;

  vkQueuePresentKHR(std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)->GetPresentQueue(), &presentInfo);
}

void VulkanCommandBuffer::CmdDraw(const ReferencePointer<VertexBuffer>& vertexBuffer) {
  vkCmdDraw(m_CommandBuffer, static_cast<uint32_t>(std::dynamic_pointer_cast<VulkanVertexBuffer>(vertexBuffer)->GetSize()), 1, 0, 0);
}

void VulkanCommandBuffer::CmdSetViewport(const ReferencePointer<SwapChain>& swapChain, uint32_t width, uint32_t height) {
  auto swapChainExtent = std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)->GetExtent();

  float viewportWidth = static_cast<float>(width);
  float viewportHeight = static_cast<float>(height);
  if (viewportWidth == UINT32_MAX) {
    viewportWidth = static_cast<float>(swapChainExtent.width);
  }
  if (viewportHeight == UINT32_MAX) {
    viewportHeight = static_cast<float>(swapChainExtent.height);
  }

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = viewportWidth;
  viewport.height = viewportHeight;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapChainExtent;
  vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);
}
