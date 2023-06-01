#include <Hydrogen/Renderer/RendererAPI.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

ReferencePointer<RendererAPI> RendererAPI::Create() {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReferencePointer<OpenGL::OpenGLRendererAPI>();
    case RendererAPI::API::Vulkan:
        return NewReferencePointer<Vulkan::VulkanRendererAPI>();
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
