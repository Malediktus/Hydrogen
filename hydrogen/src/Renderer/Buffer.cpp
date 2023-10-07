#include <Hydrogen/Renderer/Buffer.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/RendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanBuffer.hpp>
#include <Hydrogen/Core/Assert.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<VertexBuffer> VertexBuffer::Create(const ReferencePointer<RenderDevice>& device, float* vertices, size_t size) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanVertexBuffer>(device, vertices, size);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}

ReferencePointer<IndexBuffer> IndexBuffer::Create(const ReferencePointer<RenderDevice>& device, uint32_t* indices, size_t size) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanIndexBuffer>(device, indices, size);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}

ReferencePointer<UniformBuffer> UniformBuffer::Create(const ReferencePointer<RenderDevice>& device, size_t size) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanUniformBuffer>(device, size);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
