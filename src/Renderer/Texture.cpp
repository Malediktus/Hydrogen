#include <Hydrogen/Renderer/Texture.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLTexture.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

Reference<Texture2D> Texture2D::Create(const int width, const int height, Texture2DUsageType usageType) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReference<OpenGL::OpenGLTexture2D>(width, height, usageType);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}

Reference<Texture2D> Texture2D::Create(const int width, const int height, const void* data, Texture2DUsageType usageType) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReference<OpenGL::OpenGLTexture2D>(width, height, data, usageType);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
