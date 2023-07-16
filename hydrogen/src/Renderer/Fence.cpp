#include <Hydrogen/Platform/Vulkan/VulkanFence.hpp>
#include <Hydrogen/Renderer/Fence.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<Fence> Fence::Create(const ReferencePointer<RenderDevice>& renderDevice) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanFence>(renderDevice);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
