#pragma once

#include "Core.hpp"
#include "Texture.hpp"
#include "Renderbuffer.hpp"
#include "Window.hpp"
#include <memory>

namespace Vortex {
class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    virtual VT_API void Bind() const = 0;
    virtual VT_API void Unbind() const = 0;

    virtual VT_API void AttachColorTexture(const std::shared_ptr<Texture2D>& texture) = 0;
    virtual VT_API void AttachDepthTexture(const std::shared_ptr<Texture2D>& texture) = 0;
    virtual VT_API void AttachStencilTexture(const std::shared_ptr<Texture2D>& texture) = 0;
    virtual VT_API void AttachDepthStencilTexture(const std::shared_ptr<Texture2D>& texture) = 0;

    virtual VT_API void AttachColorRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) = 0;
    virtual VT_API void AttachDepthRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) = 0;
    virtual VT_API void AttachStencilRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) = 0;
    virtual VT_API void AttachDepthStencilRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) = 0;
};

VT_API std::shared_ptr<Framebuffer> FramebufferCreate(const std::shared_ptr<Window>& window);
} // namespace Vortex
