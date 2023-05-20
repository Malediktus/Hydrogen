#include "../../Renderer/Framebuffer.hpp"
#include <unordered_map>

namespace Hydrogen::OpenGL {
class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(const Reference<RenderWindow>& window);
    virtual ~OpenGLFramebuffer();

    virtual void Bind() const override;
    virtual void BindWrite() const override;
    virtual void BindRead() const override;
    virtual void Unbind() const override;

    virtual void AttachColorTexture(const Reference<Texture2D>& texture) override;
    virtual void AttachDepthTexture(const Reference<Texture2D>& texture) override;
    virtual void AttachStencilTexture(const Reference<Texture2D>& texture) override;
    virtual void AttachDepthStencilTexture(const Reference<Texture2D>& texture) override;

    virtual void AttachColorRenderbuffer(const Reference<Renderbuffer>& renderbuffer) override;
    virtual void AttachDepthRenderbuffer(const Reference<Renderbuffer>& renderbuffer) override;
    virtual void AttachStencilRenderbuffer(const Reference<Renderbuffer>& renderbuffer) override;
    virtual void AttachDepthStencilRenderbuffer(const Reference<Renderbuffer>& renderbuffer) override;

private:
    uint32_t m_RendererID;
    uint32_t numAttachments;
    uint32_t numColorAttachments = 0, numDepthAttachments = 0, numStencilAttachments = 0, numDepthStencilAttachments = 0;
};
} // namespace Hydrogen::OpenGL
