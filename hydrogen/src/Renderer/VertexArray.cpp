#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanVertexArray.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/VertexArray.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<VertexArray> VertexArray::Create() {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanVertexArray>();
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
