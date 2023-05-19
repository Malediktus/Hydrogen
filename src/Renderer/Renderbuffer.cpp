#include <Hydrogen/Renderer/Renderbuffer.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLRenderbuffer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

Reference<Renderbuffer> Renderbuffer::Create(uint32_t width, uint32_t height, RenderbufferUsageType usageType) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReference<OpenGL::OpenGLRenderbuffer>(width, height, usageType);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
