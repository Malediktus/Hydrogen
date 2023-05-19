#include <Hydrogen/Renderer/VertexArray.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLVertexArray.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

Reference<VertexArray> VertexArray::Create() {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReference<OpenGL::OpenGLVertexArray>();
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
