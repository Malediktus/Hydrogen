#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanFence.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanFence::VulkanFence(const ReferencePointer<RenderDevice>& renderDevice) : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(renderDevice)) {
  ZoneScoped;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

  VK_CHECK_ERROR(vkCreateFence(m_RenderDevice->GetDevice(), &fenceInfo, nullptr, &m_Fence), "Failed to create vulkan fence");
}

VulkanFence::~VulkanFence() {
  ZoneScoped;
  vkDestroyFence(m_RenderDevice->GetDevice(), m_Fence, nullptr);
}

void VulkanFence::Wait() { vkWaitForFences(m_RenderDevice->GetDevice(), 1, &m_Fence, VK_TRUE, UINT64_MAX); }

void VulkanFence::Reset() { vkResetFences(m_RenderDevice->GetDevice(), 1, &m_Fence); }
