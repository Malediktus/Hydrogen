#include <Hydrogen/Renderer/Context.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLContext.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<Context> Context::Create(const ReferencePointer<Hydrogen::RenderWindow>& window, ProjectInformation projectInfo) {
    ZoneScoped;

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReferencePointer<OpenGL::OpenGLContext>(window, projectInfo);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
