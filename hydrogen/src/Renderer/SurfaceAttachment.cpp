#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanSurfaceAttachment.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/SurfaceAttachment.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<SurfaceAttachment> SurfaceAttachment::Create(RenderWindow* window, bool verticalSync) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanSurfaceAttachment>(window, verticalSync);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
