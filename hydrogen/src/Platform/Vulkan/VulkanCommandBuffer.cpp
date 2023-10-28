#include <backends/imgui_impl_vulkan.h>

#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Core/Base.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanCommandBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanShader.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanSurfaceAttachment.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanVertexArray.hpp>
#include <Hydrogen/Renderer/RenderWindow.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;
using namespace Hydrogen::Vulkan;

VulkanCommandBuffer::VulkanCommandBuffer(const ReferencePointer<class RenderWindow>& window) : m_Window(window), m_ImageIndex(0) {
  ZoneScoped;

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = Renderer::GetRenderDevice<VulkanRenderDevice>()->GetCommandPool();
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = 1;

  VK_CHECK_ERROR(vkAllocateCommandBuffers(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &allocInfo, &m_CommandBuffer), "Failed to allocate vulkan command buffer!");

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VK_CHECK_ERROR(vkCreateSemaphore(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphore),
                 "Failed to create vulkan semaphore!");
  VK_CHECK_ERROR(vkCreateSemaphore(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphore),
                 "Failed to create vulkan semaphore!");

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

  VK_CHECK_ERROR(vkCreateFence(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &fenceInfo, nullptr, &m_InFlightFence), "Failed to create vulkan fence!");
  vkResetFences(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), 1, &m_InFlightFence);
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
  ZoneScoped;
  vkWaitForFences(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), 1, &m_InFlightFence, VK_TRUE, UINT64_MAX);
  vkResetFences(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), 1, &m_InFlightFence);

  vkDestroySemaphore(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_ImageAvailableSemaphore, nullptr);
  vkDestroySemaphore(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_RenderFinishedSemaphore, nullptr);
  vkDestroyFence(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_InFlightFence, nullptr);
}

void VulkanCommandBuffer::Reset() {
  ZoneScoped;
  if (vkWaitForFences(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), 1, &m_InFlightFence, VK_TRUE, 500000000) == VK_TIMEOUT) {
    HY_LOG_WARN("vkWaitForFences timed out!");
  }

  vkResetFences(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), 1, &m_InFlightFence);
  vkResetCommandBuffer(m_CommandBuffer, 0);
}

void VulkanCommandBuffer::Begin() {
  ZoneScoped;
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;                   // Optional
  beginInfo.pInheritanceInfo = nullptr;  // Optional

  VK_CHECK_ERROR(vkBeginCommandBuffer(m_CommandBuffer, &beginInfo), "Failed to begin vulkan command buffer!");
}

void VulkanCommandBuffer::End() {
  ZoneScoped;
  VK_CHECK_ERROR(vkEndCommandBuffer(m_CommandBuffer), "Failed to end vulkan command buffer!");
}

void VulkanCommandBuffer::CmdUploadResources() {
  ZoneScoped;

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {m_ImageAvailableSemaphore};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &m_CommandBuffer;

  VkSemaphore signalSemaphores[] = {m_RenderFinishedSemaphore};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  VK_CHECK_ERROR(vkQueueSubmit(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetGraphicsQueue(), 1, &submitInfo, m_InFlightFence), "Failed to submit vulkan graphics queue!");
}

void VulkanCommandBuffer::CmdDisplayImage() {
  ZoneScoped;

  VkSemaphore waitSemaphores[] = {m_RenderFinishedSemaphore};

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = waitSemaphores;

  VkSwapchainKHR swapChains[] = {m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetSwapChain()};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &m_ImageIndex;

  vkQueuePresentKHR(m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetPresentQueue(), &presentInfo);
}

void VulkanCommandBuffer::CmdDraw(const ReferencePointer<VertexBuffer>& vertexBuffer) {
  ZoneScoped;
  vkCmdDraw(m_CommandBuffer, static_cast<uint32_t>(std::dynamic_pointer_cast<VulkanVertexBuffer>(vertexBuffer)->GetSize()), 1, 0, 0);
}

void VulkanCommandBuffer::CmdDrawIndexed(const ReferencePointer<class VertexArray>& vertexArray) {
  ZoneScoped;
  vkCmdDrawIndexed(m_CommandBuffer, static_cast<uint32_t>(vertexArray->GetIndexBuffer()->GetCount()), 1, 0, 0, 0);
}

void VulkanCommandBuffer::CmdSetViewport(uint32_t width, uint32_t height) {
  ZoneScoped;

  auto swapChainExtent = m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetExtent();

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
}

void VulkanCommandBuffer::CmdSetScissor(int offsetX, int offsetY) {
  ZoneScoped;
  VkRect2D scissor{};
  scissor.offset = {offsetX, offsetY};
  scissor.extent = m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetExtent();
  vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);
}

void VulkanCommandBuffer::CmdDrawImGuiDrawData(const ReferencePointer<Shader>& shader) {
  VkPipeline pipeline = nullptr;
  if (shader != nullptr) pipeline = std::dynamic_pointer_cast<VulkanShader>(shader)->GetPipeline();

  ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_CommandBuffer, pipeline);
}
