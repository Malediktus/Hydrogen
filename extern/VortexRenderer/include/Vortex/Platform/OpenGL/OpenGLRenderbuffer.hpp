#include "../../Renderbuffer.hpp"

namespace Vortex::OpenGL {
class OpenGLRenderbuffer : public Renderbuffer {
public:
    OpenGLRenderbuffer(uint32_t width, uint32_t height, Renderbuffer::RenderbufferUsageType usageType);
    virtual ~OpenGLRenderbuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void Resize(uint32_t width, uint32_t height) override;

    virtual void* GetNative() const override;

private:
    uint32_t m_RendererID;
    uint32_t m_Format;
};
} // namespace Vortex::OpenGL
