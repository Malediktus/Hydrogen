#include <Hydrogen/Platform/Vulkan/VulkanTexture.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/Texture.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<Texture2D> Texture2D::Create(
    const int width, const int height, Texture2DStorageType storageType) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanTexture2D>(width, height,
                                                          storageType);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}

ReferencePointer<Texture2D> Texture2D::Create(
    const int width, const int height, const void* data,
    Texture2DStorageType storageType) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanTexture2D>(width, height, data,
                                                          storageType);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
