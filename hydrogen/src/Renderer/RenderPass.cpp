#include <Hydrogen/Platform/Vulkan/VulkanRenderPass.hpp>
#include <Hydrogen/Renderer/RenderPass.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<RenderPass> RenderPass::Create(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanRenderPass>(renderDevice, swapChain);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
