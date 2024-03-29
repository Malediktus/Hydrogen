#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanFramebuffer.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Core/Base.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanRendererAPI::VulkanRendererAPI(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<Framebuffer>& framebuffer)
    : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(renderDevice)), m_Framebuffer(std::dynamic_pointer_cast<VulkanFramebuffer>(framebuffer)) {}

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

  VK_CHECK_ERROR(vkCreateDescriptorPool(m_RenderDevice->GetDevice(), &createInfo, nullptr, &m_ImGuiPool), "Failed to create imgui descriptor pool");

  ImGui_ImplVulkan_InitInfo initInfo = {};
  initInfo.Instance = context->GetInstance();
  initInfo.PhysicalDevice = m_RenderDevice->GetPhysicalDevice();
  initInfo.Device = m_RenderDevice->GetDevice();
  initInfo.Queue = m_RenderDevice->GetGraphicsQueue();
  initInfo.DescriptorPool = m_ImGuiPool;
  initInfo.MinImageCount = 3;
  initInfo.ImageCount = 3;
  initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

  ImGui_ImplVulkan_Init(&initInfo, m_Framebuffer->GetRenderPass());
  
  // Upload fonts and textures to gpu
  VkFence uploadFence;
  VkCommandBuffer commandBuffer;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

  VK_CHECK_ERROR(vkCreateFence(m_RenderDevice->GetDevice(), &fenceInfo, nullptr, &uploadFence), "Failed to create vulkan fence!");

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = m_RenderDevice->GetCommandPool();
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = 1;

  VK_CHECK_ERROR(vkAllocateCommandBuffers(m_RenderDevice->GetDevice(), &allocInfo, &commandBuffer), "Failed to allocate vulkan command buffer!");

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

  VK_CHECK_ERROR(vkQueueSubmit(m_RenderDevice->GetGraphicsQueue(), 1, &submitInfo, uploadFence), "Failed to submit to vulkan graphics queue!");

  vkWaitForFences(m_RenderDevice->GetDevice(), 1, &uploadFence, true, 9999999999);
  vkResetFences(m_RenderDevice->GetDevice(), 1, &uploadFence);

  vkDestroyFence(m_RenderDevice->GetDevice(), uploadFence, nullptr);
  vkFreeCommandBuffers(m_RenderDevice->GetDevice(), m_RenderDevice->GetCommandPool(), 1, &commandBuffer);

  // Free fonts and textures from cpu memory
  ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void VulkanRendererAPI::ImGuiNewFrame() { ImGui_ImplVulkan_NewFrame(); }

void VulkanRendererAPI::DestroyImGui() {
  vkDestroyDescriptorPool(m_RenderDevice->GetDevice(), m_ImGuiPool, nullptr);
  ImGui_ImplVulkan_Shutdown();
}
