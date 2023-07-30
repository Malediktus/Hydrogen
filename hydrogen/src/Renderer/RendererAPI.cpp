#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/RendererAPI.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

ReferencePointer<RendererAPI> RendererAPI::Create() {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanRendererAPI>();
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
