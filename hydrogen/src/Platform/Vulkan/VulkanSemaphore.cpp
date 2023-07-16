#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanSemaphore.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanSemaphore::VulkanSemaphore(const ReferencePointer<RenderDevice>& renderDevice) : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(renderDevice)) {
  ZoneScoped;

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VK_CHECK_ERROR(vkCreateSemaphore(m_RenderDevice->GetDevice(), &semaphoreInfo, nullptr, &m_Semaphore), "Failed to create vulkan semaphore");
}

VulkanSemaphore::~VulkanSemaphore() {
  ZoneScoped;
  vkDestroySemaphore(m_RenderDevice->GetDevice(), m_Semaphore, nullptr);
}
