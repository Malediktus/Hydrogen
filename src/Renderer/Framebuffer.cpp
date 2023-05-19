#include <Hydrogen/Renderer/Framebuffer.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLFramebuffer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

Reference<Framebuffer> Framebuffer::Create(const Reference<RenderWindow>& window) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReference<OpenGL::OpenGLFramebuffer>(window);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
