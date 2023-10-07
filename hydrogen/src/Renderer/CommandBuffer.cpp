#include <Hydrogen/Platform/Vulkan/VulkanCommandBuffer.hpp>
#include <Hydrogen/Renderer/CommandBuffer.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Core/Assert.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<CommandBuffer> CommandBuffer::Create(const ReferencePointer<RenderDevice>& renderDevice) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanCommandBuffer>(renderDevice);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
