#include "../../Framebuffer.hpp"
#include <unordered_map>

namespace Vortex::OpenGL {
class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(const std::shared_ptr<Window>& window);
    virtual ~OpenGLFramebuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AttachColorTexture(const std::shared_ptr<Texture2D>& texture) override;
    virtual void AttachDepthTexture(const std::shared_ptr<Texture2D>& texture) override;
    virtual void AttachStencilTexture(const std::shared_ptr<Texture2D>& texture) override;
    virtual void AttachDepthStencilTexture(const std::shared_ptr<Texture2D>& texture) override;

    virtual void AttachColorRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) override;
    virtual void AttachDepthRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) override;
    virtual void AttachStencilRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) override;
    virtual void AttachDepthStencilRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) override;

private:
    uint32_t m_RendererID;
    bool hasAttachments;
};
} // namespace Vortex::OpenGL
