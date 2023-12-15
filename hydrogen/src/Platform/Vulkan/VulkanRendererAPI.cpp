#include <backends/imgui_impl_vulkan.h>
#include <imgui.h>

#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Core/Base.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanSurfaceAttachment.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanVertexArray.hpp>
#include <Hydrogen/Renderer/Buffer.hpp>
#include <Hydrogen/Renderer/RenderWindow.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

// TODO: Multiple frames in flights
VulkanRendererAPI::VulkanRendererAPI(const ReferencePointer<class RenderWindow>& window) : m_Window(window), m_ImageIndex(0) {
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

VulkanRendererAPI::~VulkanRendererAPI() {
  ZoneScoped;

  vkWaitForFences(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), 1, &m_InFlightFence, VK_TRUE, UINT64_MAX);
  vkResetFences(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), 1, &m_InFlightFence);

  vkDestroySemaphore(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_ImageAvailableSemaphore, nullptr);
  vkDestroySemaphore(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_RenderFinishedSemaphore, nullptr);
  vkDestroyFence(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_InFlightFence, nullptr);
}

void VulkanRendererAPI::SetupImGui() {
  const auto& context = Renderer::GetContext<VulkanContext>();

  VkDescriptorPoolSize poolSizes[] = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                                      {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                                      {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
                                      {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                                      {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                                      {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                                      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                                      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                                      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                                      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                                      {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

  VkDescriptorPoolCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  createInfo.maxSets = 1000;
  createInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
  createInfo.pPoolSizes = poolSizes;

  VK_CHECK_ERROR(vkCreateDescriptorPool(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &createInfo, nullptr, &m_ImGuiPool),
                 "Failed to create imgui descriptor pool");

  ImGui_ImplVulkan_InitInfo initInfo = {};
  initInfo.Instance = context->GetInstance();
  initInfo.PhysicalDevice = Renderer::GetRenderDevice<VulkanRenderDevice>()->GetPhysicalDevice();
  initInfo.Device = Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice();
  initInfo.Queue = Renderer::GetRenderDevice<VulkanRenderDevice>()->GetGraphicsQueue();
  initInfo.DescriptorPool = m_ImGuiPool;
  initInfo.MinImageCount = 3;
  initInfo.ImageCount = 3;
  initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

  ImGui_ImplVulkan_Init(&initInfo, m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetRenderPass());

  // Upload fonts and textures to gpu
  VkFence uploadFence;
  VkCommandBuffer commandBuffer;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

  VK_CHECK_ERROR(vkCreateFence(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &fenceInfo, nullptr, &uploadFence), "Failed to create vulkan fence!");

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = Renderer::GetRenderDevice<VulkanRenderDevice>()->GetCommandPool();
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = 1;

  VK_CHECK_ERROR(vkAllocateCommandBuffers(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), &allocInfo, &commandBuffer), "Failed to allocate vulkan command buffer!");

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.pNext = nullptr;
  beginInfo.pInheritanceInfo = nullptr;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  VK_CHECK_ERROR(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Failed to begin vulkan command buffer!");

  ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

  VK_CHECK_ERROR(vkEndCommandBuffer(commandBuffer), "Failed to end vulkan command buffer!");

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.pNext = nullptr;
  submitInfo.waitSemaphoreCount = 0;
  submitInfo.pWaitSemaphores = nullptr;
  submitInfo.pWaitDstStageMask = nullptr;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;
  submitInfo.signalSemaphoreCount = 0;
  submitInfo.pSignalSemaphores = nullptr;

  VK_CHECK_ERROR(vkQueueSubmit(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetGraphicsQueue(), 1, &submitInfo, uploadFence), "Failed to submit to vulkan graphics queue!");

  vkWaitForFences(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), 1, &uploadFence, true, 9999999999);
  vkResetFences(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), 1, &uploadFence);

  vkDestroyFence(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), uploadFence, nullptr);
  vkFreeCommandBuffers(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), Renderer::GetRenderDevice<VulkanRenderDevice>()->GetCommandPool(), 1, &commandBuffer);

  // Free fonts and textures from cpu memory
  ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void VulkanRendererAPI::ImGuiNewFrame() {
  ZoneScoped;
  ImGui_ImplVulkan_NewFrame();
}

void VulkanRendererAPI::DestroyImGui() {
  vkDestroyDescriptorPool(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_ImGuiPool, nullptr);
  ImGui_ImplVulkan_Shutdown();
}

void VulkanRendererAPI::BeginFrame() {
  ZoneScoped;
  if (vkWaitForFences(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), 1, &m_InFlightFence, VK_TRUE, 500000000) == VK_TIMEOUT) {
    HY_LOG_WARN("vkWaitForFences timed out!");
  }

  vkResetFences(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), 1, &m_InFlightFence);
  vkResetCommandBuffer(m_CommandBuffer, 0);

  vkAcquireNextImageKHR(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(), m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetSwapChain(), UINT64_MAX, m_ImageAvailableSemaphore, VK_NULL_HANDLE, &m_ImageIndex);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS) {
    HY_INVOKE_ERROR("Failed to begin vulkan recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetRenderPass();
  renderPassInfo.framebuffer = m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetFramebuffers()[m_ImageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};

  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanRendererAPI::EndFrame() {
  vkCmdEndRenderPass(m_CommandBuffer);

  if (vkEndCommandBuffer(m_CommandBuffer) != VK_SUCCESS) {
    HY_INVOKE_ERROR("Failed to record vulkan command buffer!");
  }

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

  if (vkQueueSubmit(Renderer::GetRenderDevice<VulkanRenderDevice>()->GetGraphicsQueue(), 1, &submitInfo, m_InFlightFence) != VK_SUCCESS) {
    HY_INVOKE_ERROR("Failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = {m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetSwapChain()};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;

  presentInfo.pImageIndices = &m_ImageIndex;

  vkQueuePresentKHR(m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetPresentQueue(), &presentInfo);
}

void VulkanRendererAPI::DrawIndexed(const ReferencePointer<VertexArray>& vertexArray) {
  ZoneScoped;
  vkCmdDrawIndexed(m_CommandBuffer, static_cast<uint32_t>(vertexArray->GetIndexBuffer()->GetCount()), 1, 0, 0, 0);
}

void VulkanRendererAPI::SetViewport(uint32_t width, uint32_t height) {
  const auto swapChainExtent = m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetExtent();

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)swapChainExtent.width;
  viewport.height = (float)swapChainExtent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);
}

void VulkanRendererAPI::SetScissor(int offsetX, int offsetY) {
  const auto swapChainExtent = m_Window->GetSurfaceAttachment<VulkanSurfaceAttachment>()->GetExtent();

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapChainExtent;
  vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);
}
