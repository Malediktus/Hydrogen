#pragma once

#include "../Core/Base.hpp"
#include "../Core/Assert.hpp"
#include "../Core/Memory.hpp"
#include "Texture.hpp"
#include <memory>

namespace Hydrogen {
enum class RenderbufferUsageType { Color = 0, Depth = 1, Stencil = 2, DepthStencil = 3 };

class Renderbuffer {
public:
    virtual ~Renderbuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual void* GetNative() const = 0;

    static Reference<Renderbuffer> Create(uint32_t width, uint32_t height, RenderbufferUsageType usageType);
};

} // namespace Hydrogen
