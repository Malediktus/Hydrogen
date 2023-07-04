#pragma once

#include "../Core/Base.hpp"
#include "../Core/Assert.hpp"
#include "../Core/Memory.hpp"
#include "Texture.hpp"
#include "RenderWindow.hpp"
#include <memory>

namespace Hydrogen {
enum class FramebufferAttachment { Color = 0, Depth = 1, Stencil = 2, DepthStencil = 3 };

class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    virtual void Bind() const = 0;
    virtual void BindWrite() const = 0;
    virtual void BindRead() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetDrawBuffers(const DynamicArray<std::pair<FramebufferAttachment, uint32_t>>& attachments) = 0;

    virtual void AttachColorTexture(const ReferencePointer<Texture2D>& texture) = 0;
    virtual void AttachDepthTexture(const ReferencePointer<Texture2D>& texture) = 0;
    virtual void AttachStencilTexture(const ReferencePointer<Texture2D>& texture) = 0;
    virtual void AttachDepthStencilTexture(const ReferencePointer<Texture2D>& texture) = 0;

    static ReferencePointer<Framebuffer> Create(const ReferencePointer<RenderWindow>& window);
};
} // namespace Hydrogen
