#include "../../Renderer/Framebuffer.hpp"
#include <unordered_map>

namespace Hydrogen::OpenGL {
class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(const ReferencePointer<RenderWindow>& window);
    virtual ~OpenGLFramebuffer();

    virtual void Bind() const override;
    virtual void BindWrite() const override;
    virtual void BindRead() const override;
    virtual void Unbind() const override;

    virtual void SetDrawBuffers(const std::vector<std::pair<FramebufferAttachment, uint32_t>>& attachments) override;

    virtual void AttachColorTexture(const ReferencePointer<Texture2D>& texture) override;
    virtual void AttachDepthTexture(const ReferencePointer<Texture2D>& texture) override;
    virtual void AttachStencilTexture(const ReferencePointer<Texture2D>& texture) override;
    virtual void AttachDepthStencilTexture(const ReferencePointer<Texture2D>& texture) override;

    virtual void AttachColorRenderbuffer(const ReferencePointer<Renderbuffer>& renderbuffer) override;
    virtual void AttachDepthRenderbuffer(const ReferencePointer<Renderbuffer>& renderbuffer) override;
    virtual void AttachStencilRenderbuffer(const ReferencePointer<Renderbuffer>& renderbuffer) override;
    virtual void AttachDepthStencilRenderbuffer(const ReferencePointer<Renderbuffer>& renderbuffer) override;

private:
    uint32_t m_RendererID;
    uint32_t numAttachments;
    uint32_t numColorAttachments = 0, numDepthAttachments = 0, numStencilAttachments = 0, numDepthStencilAttachments = 0;
};
} // namespace Hydrogen::OpenGL
