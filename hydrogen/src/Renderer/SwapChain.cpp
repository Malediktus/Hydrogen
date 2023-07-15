#include <Hydrogen/Platform/Vulkan/VulkanSwapChain.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/SwapChain.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<SwapChain> SwapChain::Create(bool verticalSync) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanSwapChain>(verticalSync);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
