#include <Hydrogen/Renderer/Buffer.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanBuffer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
        return NewReferencePointer<Vulkan::VulkanVertexBuffer>(vertices, size);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}

ReferencePointer<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
        return NewReferencePointer<Vulkan::VulkanVertexBuffer>(size);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}

ReferencePointer<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
        return NewReferencePointer<Vulkan::VulkanIndexBuffer>(indices, size);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
