#include <Vortex/Platform/OpenGL/OpenGLRenderbuffer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

using namespace Vortex::OpenGL;

OpenGLRenderbuffer::OpenGLRenderbuffer(uint32_t width, uint32_t height, Renderbuffer::RenderbufferUsageType usageType) {
    ZoneScoped;
    glGenRenderbuffers(1, &m_RendererID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    GLuint format = GL_RGBA8;

    switch (usageType) {
    case Renderbuffer::RenderbufferUsageType::Color:
        format = GL_RGBA8;
        break;
    case Renderbuffer::RenderbufferUsageType::Depth:
        format = GL_DEPTH_COMPONENT32F;
        break;
    case Renderbuffer::RenderbufferUsageType::Stencil:
        format = GL_STENCIL_INDEX8;
        break;
    case Renderbuffer::RenderbufferUsageType::DepthStencil:
        format = GL_DEPTH24_STENCIL8;
        break;
    }

    m_Format = format;

    glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glCheckError();
    spdlog::trace("Created OpenGL renderbuffer (ID: {})", m_RendererID);
}

OpenGLRenderbuffer::~OpenGLRenderbuffer() {
    ZoneScoped;
    glDeleteRenderbuffers(1, &m_RendererID);
    glCheckError();
    spdlog::trace("Deleted OpenGL renderbuffer (ID: {})", m_RendererID);
}

void OpenGLRenderbuffer::Bind() const {
    ZoneScoped;
    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    glCheckError();
    spdlog::trace("Bound OpenGL renderbuffer (ID: {})", m_RendererID);
}

void OpenGLRenderbuffer::Unbind() const {
    ZoneScoped;
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glCheckError();
    spdlog::trace("Unbound OpenGL renderbuffer (ID: {})", m_RendererID);
}

void OpenGLRenderbuffer::Resize(uint32_t width, uint32_t height) {
    ZoneScoped;
    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    glRenderbufferStorage(GL_RENDERBUFFER, m_Format, width, height);
    glCheckError();
    spdlog::trace("Resized OpenGL renderbuffer (ID: {})", m_RendererID);
}

void* OpenGLRenderbuffer::GetNative() const {
    return (void*) &m_RendererID;
}
