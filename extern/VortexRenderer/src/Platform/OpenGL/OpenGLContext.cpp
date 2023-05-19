#include <glad/glad.h>
#include <Vortex/Platform/OpenGL/OpenGLContext.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

using namespace Vortex::OpenGL;

void OpenGLContext::Init() {
    ZoneScoped;
    m_Window->SetupOpenglContext(3, 3);
    VT_ASSERT(gladLoadGLLoader((GLADloadproc) m_Window->GetWindowOpenGLProcAddress()), "Failed to initialize OpenGL Context");

    spdlog::info("Created OpenGL context");
    int majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    spdlog::info("Using OpenGL version {}.{}", majorVersion, minorVersion);
}
