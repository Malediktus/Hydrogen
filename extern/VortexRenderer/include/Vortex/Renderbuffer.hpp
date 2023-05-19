#pragma once

#include "Core.hpp"
#include "Texture.hpp"
#include <memory>

namespace Vortex {
class Renderbuffer {
public:
    enum class RenderbufferUsageType { Color = 0, Depth = 1, Stencil = 2, DepthStencil = 3 };

    virtual ~Renderbuffer() = default;

    virtual VT_API void Bind() const = 0;
    virtual VT_API void Unbind() const = 0;

    virtual VT_API void Resize(uint32_t width, uint32_t height) = 0;

    virtual void* GetNative() const = 0;
};

VT_API std::shared_ptr<Renderbuffer> RenderbufferCreate(uint32_t width, uint32_t height, Renderbuffer::RenderbufferUsageType usageType);
} // namespace Vortex
