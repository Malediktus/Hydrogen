#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanBuffer.hpp>
#include <Hydrogen/Renderer/Buffer.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/RendererAPI.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanVertexBuffer>(vertices, size);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}

ReferencePointer<IndexBuffer> IndexBuffer::Create(uint32_t* indices, size_t size) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanIndexBuffer>(indices, size);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}

ReferencePointer<UniformBuffer> UniformBuffer::Create(size_t size) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanUniformBuffer>(size);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
