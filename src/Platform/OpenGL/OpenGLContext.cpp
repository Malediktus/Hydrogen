#include <Hydrogen/Platform/OpenGL/OpenGLContext.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <glad/glad.h>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::OpenGL;

OpenGLContext::OpenGLContext(const ReferencePointer<RenderWindow>& window) : m_Window(window) {
}

void OpenGLContext::Init(ProjectInformation, ProjectInformation) {
    ZoneScoped;
    m_Window->SetupOpenglContext(3, 3);
    HY_ASSERT(gladLoadGLLoader((GLADloadproc) m_Window->GetWindowOpenGLProcAddress()), "Failed to initialize OpenGL Context");

    HY_LOG_INFO("Created OpenGL context");
    int majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    HY_LOG_INFO("Using OpenGL version {}.{}", majorVersion, minorVersion);
}
