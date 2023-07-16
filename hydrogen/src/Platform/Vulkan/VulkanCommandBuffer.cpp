#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanCommandBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;
using namespace Hydrogen::Vulkan;

VulkanCommandBuffer::VulkanCommandBuffer(const ReferencePointer<RenderDevice>& renderDevice) : m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(renderDevice)) {}

VulkanCommandBuffer::~VulkanCommandBuffer() {}

void VulkanCommandBuffer::Begin(const ReferencePointer<RenderPass>& renderPass, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Framebuffer>& framebuffer) {}

void VulkanCommandBuffer::End() {}

void VulkanCommandBuffer::CmdDraw(const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Shader>& shader) {}
