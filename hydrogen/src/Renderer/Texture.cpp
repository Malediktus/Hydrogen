#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanTexture.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/Texture.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<Texture2D> Texture2D::Create(const ReferencePointer<class RenderWindow>& window, const uint32_t width, const uint32_t height, const void* data) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanTexture2D>(window, width, height, data);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
