#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderPass.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

VulkanRenderPass::VulkanRenderPass(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain)
    : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(renderDevice)), m_SwapChain(std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)) {
  ZoneScoped;
}

VulkanRenderPass::~VulkanRenderPass() { ZoneScoped; }
