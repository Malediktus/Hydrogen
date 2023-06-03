#include <Hydrogen/Renderer/Context.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLContext.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<Context> Context::Create(const ReferencePointer<Hydrogen::RenderWindow>& window, ProjectInformation clientInfo, ProjectInformation engineInfo) {
    ZoneScoped;

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReferencePointer<OpenGL::OpenGLContext>(window, clientInfo, engineInfo);
    case RendererAPI::API::Vulkan:
        return NewReferencePointer<Vulkan::VulkanContext>(window, clientInfo, engineInfo);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
