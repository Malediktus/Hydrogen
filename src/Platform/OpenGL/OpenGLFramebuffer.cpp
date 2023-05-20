#include <Hydrogen/Platform/OpenGL/OpenGLFramebuffer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <glad/glad.h>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;
using namespace Hydrogen::OpenGL;

OpenGLFramebuffer::OpenGLFramebuffer(const std::shared_ptr<RenderWindow>&) {
    ZoneScoped;
    glGenFramebuffers(1, &m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Created OpenGL framebuffer (ID: {})", m_RendererID);
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    ZoneScoped;
    glDeleteFramebuffers(1, &m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Deleted OpenGL framebuffer (ID: {})", m_RendererID);
}

void OpenGLFramebuffer::Bind() const {
    ZoneScoped;
    if (numAttachments == 0)
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Bound OpenGL framebuffer (ID: {})", m_RendererID);
}

void OpenGLFramebuffer::BindWrite() const {
    ZoneScoped;
    if (numAttachments == 0)
        return;

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Bound OpenGL framebuffer for drawing (ID: {})", m_RendererID);
}

void OpenGLFramebuffer::BindRead() const {
    ZoneScoped;
    if (numAttachments == 0)
        return;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Bound OpenGL framebuffer for reading (ID: {})", m_RendererID);
}

void OpenGLFramebuffer::Unbind() const {
    ZoneScoped;
    if (numAttachments == 0)
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    HY_LOG_TRACE("Unbound OpenGL framebuffer (ID: {})", m_RendererID);
}

void OpenGLFramebuffer::AttachColorTexture(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
    HY_ASSERT(numColorAttachments < 32, "Too many color attachments");
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + numColorAttachments, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    Unbind();
    glCheckError();
    numColorAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached texture of type color buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachDepthTexture(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
    HY_ASSERT(numDepthAttachments < 1 && numDepthStencilAttachments < 1, "Too many depth attachments");
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    Unbind();
    glCheckError();
    numDepthAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached texture of type depth buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachStencilTexture(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
    HY_ASSERT(numStencilAttachments < 1 && numDepthStencilAttachments < 1, "Too many stencil attachments");
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    Unbind();
    glCheckError();
    numStencilAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached texture of type stencil buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachDepthStencilTexture(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
    HY_ASSERT(numDepthStencilAttachments < 1 && numDepthAttachments < 1 && numStencilAttachments < 1, "Too many depth/stencil attachments");
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    Unbind();
    glCheckError();
    numDepthStencilAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached texture of type depth_stencil buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachColorRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    HY_ASSERT(numColorAttachments < 32, "Too many color attachments");
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + numColorAttachments, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    Unbind();
    glCheckError();
    numColorAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached renderbuffer of type color buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}

void OpenGLFramebuffer::AttachDepthRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    HY_ASSERT(numDepthAttachments < 1 && numDepthStencilAttachments < 1, "Too many depth attachments");
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    Unbind();
    glCheckError();
    numDepthAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached renderbuffer of type depth buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}

void OpenGLFramebuffer::AttachStencilRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    HY_ASSERT(numStencilAttachments < 1 && numDepthStencilAttachments < 1, "Too many stencil attachments");
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    Unbind();
    glCheckError();
    numStencilAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached renderbuffer of type stencil buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}

void OpenGLFramebuffer::AttachDepthStencilRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    HY_ASSERT(numDepthStencilAttachments < 1 && numDepthAttachments < 1 && numStencilAttachments < 1, "Too many depth/stencil attachments");
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    Unbind();
    glCheckError();
    numDepthStencilAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached renderbuffer of type depth_stencil buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}
