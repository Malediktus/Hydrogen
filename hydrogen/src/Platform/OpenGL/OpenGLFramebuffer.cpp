#include <Hydrogen/Platform/OpenGL/OpenGLFramebuffer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <glad/glad.h>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;
using namespace Hydrogen::OpenGL;

OpenGLFramebuffer::OpenGLFramebuffer(const ReferencePointer<RenderWindow>&) {
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

void OpenGLFramebuffer::SetDrawBuffers(const DynamicArray<std::pair<FramebufferAttachment, uint32_t>>& attachments) {
    ZoneScoped;
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    DynamicArray<GLenum> vec(attachments.size());
    for (auto& attachment : attachments) {
        switch (attachment.first) {
        case FramebufferAttachment::Color:
            HY_ASSERT(attachment.second < 32, "Value of range");
            vec.push_back(GL_COLOR_ATTACHMENT0 + attachment.second);
            break;
        case FramebufferAttachment::Depth:
            vec.push_back(GL_DEPTH_ATTACHMENT);
            break;
        case FramebufferAttachment::Stencil:
            vec.push_back(GL_STENCIL_ATTACHMENT);
            break;
        case FramebufferAttachment::DepthStencil:
            vec.push_back(GL_DEPTH_STENCIL_ATTACHMENT);
            break;
        default:
            HY_INVOKE_ERROR("Invalid FramebufferAttachment value");
            break;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    HY_LOG_TRACE("Set OpenGL draw buffers for framebuffer (ID: {})", m_RendererID);
}

void OpenGLFramebuffer::AttachColorTexture(const ReferencePointer<Texture2D>& texture) {
    ZoneScoped;
    HY_ASSERT(numColorAttachments < 32, "Too many color attachments");
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + numColorAttachments, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    numColorAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached texture of type color buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachDepthTexture(const ReferencePointer<Texture2D>& texture) {
    ZoneScoped;
    HY_ASSERT(numDepthAttachments < 1 && numDepthStencilAttachments < 1, "Too many depth attachments");
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    numDepthAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached texture of type depth buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachStencilTexture(const ReferencePointer<Texture2D>& texture) {
    ZoneScoped;
    HY_ASSERT(numStencilAttachments < 1 && numDepthStencilAttachments < 1, "Too many stencil attachments");
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    numStencilAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached texture of type stencil buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachDepthStencilTexture(const ReferencePointer<Texture2D>& texture) {
    ZoneScoped;
    HY_ASSERT(numDepthStencilAttachments < 1 && numDepthAttachments < 1 && numStencilAttachments < 1, "Too many depth/stencil attachments");
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    numDepthStencilAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached texture of type depth_stencil buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachColorRenderbuffer(const ReferencePointer<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    HY_ASSERT(numColorAttachments < 32, "Too many color attachments");
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + numColorAttachments, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    numColorAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached renderbuffer of type color buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}

void OpenGLFramebuffer::AttachDepthRenderbuffer(const ReferencePointer<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    HY_ASSERT(numDepthAttachments < 1 && numDepthStencilAttachments < 1, "Too many depth attachments");
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    numDepthAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached renderbuffer of type depth buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}

void OpenGLFramebuffer::AttachStencilRenderbuffer(const ReferencePointer<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    HY_ASSERT(numStencilAttachments < 1 && numDepthStencilAttachments < 1, "Too many stencil attachments");
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    numStencilAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached renderbuffer of type stencil buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}

void OpenGLFramebuffer::AttachDepthStencilRenderbuffer(const ReferencePointer<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    HY_ASSERT(numDepthStencilAttachments < 1 && numDepthAttachments < 1 && numStencilAttachments < 1, "Too many depth/stencil attachments");
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    numDepthStencilAttachments++;
    numAttachments++;
    HY_LOG_TRACE("Attached renderbuffer of type depth_stencil buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}
