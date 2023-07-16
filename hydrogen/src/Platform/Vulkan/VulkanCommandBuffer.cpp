#include <Hydrogen/Core/Logger.hpp>
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
}

VulkanCommandBuffer::~VulkanCommandBuffer() {}

void VulkanCommandBuffer::Reset() { vkResetCommandBuffer(m_CommandBuffer, 0); }

void VulkanCommandBuffer::Begin(const ReferencePointer<RenderPass>& renderPass, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Framebuffer>& framebuffer,
                                Vector4 clearColor, uint32_t imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;                   // Optional
  beginInfo.pInheritanceInfo = nullptr;  // Optional

  VK_CHECK_ERROR(vkBeginCommandBuffer(m_CommandBuffer, &beginInfo), "Failed to begin vulkan command buffer!");

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = std::dynamic_pointer_cast<VulkanRenderPass>(renderPass)->GetRenderPass();
  renderPassInfo.framebuffer = std::dynamic_pointer_cast<VulkanFramebuffer>(framebuffer)->GetFramebuffers()[imageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)->GetExtent();

  VkClearValue vkClearColor = {{{clearColor.r, clearColor.g, clearColor.b, clearColor.a}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &vkClearColor;

  vkCmdBeginRenderPass(m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandBuffer::End() {
  vkCmdEndRenderPass(m_CommandBuffer);
  VK_CHECK_ERROR(vkEndCommandBuffer(m_CommandBuffer), "Failed to end vulkan command buffer!");
}

void VulkanCommandBuffer::CmdDraw(const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Shader>& shader) {
  vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, std::dynamic_pointer_cast<VulkanShader>(shader)->GetPipeline());

  auto swapChainExtent = std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)->GetExtent();

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapChainExtent.width);
  viewport.height = static_cast<float>(swapChainExtent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapChainExtent;
  vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);

  vkCmdDraw(m_CommandBuffer, 3, 1, 0, 0);
}

void VulkanCommandBuffer::SubmitGraphicsQueue(const ReferencePointer<Semaphore>& imageAvailableSemaphore, const ReferencePointer<Semaphore>& renderFinishedSemaphore,
                                              const ReferencePointer<Fence>& inFlightFence) {
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {std::dynamic_pointer_cast<VulkanSemaphore>(imageAvailableSemaphore)->GetSemaphore()};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &m_CommandBuffer;

  VkSemaphore signalSemaphores[] = {std::dynamic_pointer_cast<VulkanSemaphore>(renderFinishedSemaphore)->GetSemaphore()};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  VK_CHECK_ERROR(vkQueueSubmit(m_RenderDevice->GetGraphicsQueue(), 1, &submitInfo, std::dynamic_pointer_cast<VulkanFence>(inFlightFence)->GetFence()),
                 "Failed to submit vulkan graphics queue!");
}

void VulkanCommandBuffer::PresentQueue(const ReferencePointer<Semaphore>& renderFinishedSemaphore, const ReferencePointer<SwapChain>& swapChain, uint32_t* imageIndex) {
  VkSemaphore waitSemaphores[] = {std::dynamic_pointer_cast<VulkanSemaphore>(renderFinishedSemaphore)->GetSemaphore()};

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = waitSemaphores;

  VkSwapchainKHR swapChains[] = {std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)->GetSwapChain()};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = imageIndex;

  vkQueuePresentKHR(m_RenderDevice->GetPresentQueue(), &presentInfo);
}
