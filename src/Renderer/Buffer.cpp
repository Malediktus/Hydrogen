#include <Hydrogen/Renderer/Buffer.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLBuffer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

Reference<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReference<OpenGL::OpenGLVertexBuffer>(vertices, size);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}

Reference<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReference<OpenGL::OpenGLVertexBuffer>(size);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}

Reference<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReference<OpenGL::OpenGLIndexBuffer>(indices, size);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
