#pragma once

#include "../Core/Base.hpp"
#include "../Core/Assert.hpp"
#include "../Core/Memory.hpp"
#include "Texture.hpp"
#include "Renderbuffer.hpp"
#include "RenderWindow.hpp"
#include <memory>

namespace Hydrogen {
class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    virtual void Bind() const = 0;
    virtual void BindWrite() const = 0;
    virtual void BindRead() const = 0;
    virtual void Unbind() const = 0;

    virtual void AttachColorTexture(const Reference<Texture2D>& texture) = 0;
    virtual void AttachDepthTexture(const Reference<Texture2D>& texture) = 0;
    virtual void AttachStencilTexture(const Reference<Texture2D>& texture) = 0;
    virtual void AttachDepthStencilTexture(const Reference<Texture2D>& texture) = 0;

    virtual void AttachColorRenderbuffer(const Reference<Renderbuffer>& renderbuffer) = 0;
    virtual void AttachDepthRenderbuffer(const Reference<Renderbuffer>& renderbuffer) = 0;
    virtual void AttachStencilRenderbuffer(const Reference<Renderbuffer>& renderbuffer) = 0;
    virtual void AttachDepthStencilRenderbuffer(const Reference<Renderbuffer>& renderbuffer) = 0;

    static Reference<Framebuffer> Create(const Reference<RenderWindow>& window);
};
} // namespace Hydrogen
