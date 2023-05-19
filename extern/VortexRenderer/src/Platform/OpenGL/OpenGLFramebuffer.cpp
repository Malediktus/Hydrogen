#include <Vortex/Platform/OpenGL/OpenGLFramebuffer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

using namespace Vortex;
using namespace Vortex::OpenGL;

OpenGLFramebuffer::OpenGLFramebuffer(const std::shared_ptr<Window>&) {
    ZoneScoped;
    hasAttachments = false;
    glGenFramebuffers(1, &m_RendererID);
    glCheckError();
    spdlog::trace("Created OpenGL framebuffer (ID: {})", m_RendererID);
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    ZoneScoped;
    glDeleteFramebuffers(1, &m_RendererID);
    glCheckError();
    spdlog::trace("Deleted OpenGL framebuffer (ID: {})", m_RendererID);
}

void OpenGLFramebuffer::Bind() const {
    ZoneScoped;
    if (!hasAttachments)
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glCheckError();
    spdlog::trace("Bound OpenGL framebuffer (ID: {})", m_RendererID);
}

void OpenGLFramebuffer::Unbind() const {
    ZoneScoped;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    spdlog::trace("Unbound OpenGL framebuffer (ID: {})", m_RendererID);
}

void OpenGLFramebuffer::AttachColorTexture(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
    hasAttachments = true;
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    Unbind();
    glCheckError();
    spdlog::trace("Attached texture of type color buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachDepthTexture(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
    hasAttachments = true;
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    Unbind();
    glCheckError();
    spdlog::trace("Attached texture of type depth buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachStencilTexture(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
    hasAttachments = true;
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    Unbind();
    glCheckError();
    spdlog::trace("Attached texture of type stencil buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachDepthStencilTexture(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
    hasAttachments = true;
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    Unbind();
    glCheckError();
    spdlog::trace("Attached texture of type depth_stencil buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachColorRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    hasAttachments = true;
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    Unbind();
    glCheckError();
    spdlog::trace("Attached renderbuffer of type color buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}

void OpenGLFramebuffer::AttachDepthRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    hasAttachments = true;
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    Unbind();
    glCheckError();
    spdlog::trace("Attached renderbuffer of type depth buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}

void OpenGLFramebuffer::AttachStencilRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    hasAttachments = true;
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    Unbind();
    glCheckError();
    spdlog::trace("Attached renderbuffer of type stencil buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}

void OpenGLFramebuffer::AttachDepthStencilRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    hasAttachments = true;
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    Unbind();
    glCheckError();
    spdlog::trace("Attached renderbuffer of type depth_stencil buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}
