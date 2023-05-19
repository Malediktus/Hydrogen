#include <Hydrogen/Platform/OpenGL/OpenGLRenderbuffer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <glad/glad.h>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::OpenGL;

OpenGLRenderbuffer::OpenGLRenderbuffer(uint32_t width, uint32_t height, RenderbufferUsageType usageType) {
    ZoneScoped;
    glGenRenderbuffers(1, &m_RendererID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    GLuint format = GL_RGBA8;

    switch (usageType) {
    case RenderbufferUsageType::Color:
        format = GL_RGBA8;
        break;
    case RenderbufferUsageType::Depth:
        format = GL_DEPTH_COMPONENT32F;
        break;
    case RenderbufferUsageType::Stencil:
        format = GL_STENCIL_INDEX8;
        break;
    case RenderbufferUsageType::DepthStencil:
        format = GL_DEPTH24_STENCIL8;
        break;
    }

    m_Format = format;

    glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glCheckError();
    HY_LOG_TRACE("Created OpenGL renderbuffer (ID: {})", m_RendererID);
}

OpenGLRenderbuffer::~OpenGLRenderbuffer() {
    ZoneScoped;
    glDeleteRenderbuffers(1, &m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Deleted OpenGL renderbuffer (ID: {})", m_RendererID);
}

void OpenGLRenderbuffer::Bind() const {
    ZoneScoped;
    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Bound OpenGL renderbuffer (ID: {})", m_RendererID);
}

void OpenGLRenderbuffer::Unbind() const {
    ZoneScoped;
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glCheckError();
    HY_LOG_TRACE("Unbound OpenGL renderbuffer (ID: {})", m_RendererID);
}

void OpenGLRenderbuffer::Resize(uint32_t width, uint32_t height) {
    ZoneScoped;
    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    glRenderbufferStorage(GL_RENDERBUFFER, m_Format, width, height);
    glCheckError();
    HY_LOG_TRACE("Resized OpenGL renderbuffer (ID: {})", m_RendererID);
}

void* OpenGLRenderbuffer::GetNative() const {
    return (void*) &m_RendererID;
}
