#pragma once

#include "../Core/Memory.hpp"
#include "../Math/Math.hpp"

namespace Hydrogen {
class Framebuffer {
 public:
  virtual ~Framebuffer() = default;

  static ReferencePointer<Framebuffer> Create(const ReferencePointer<class RenderDevice>& renderDevice, const ReferencePointer<class SwapChain>& swapChain);

  virtual void Bind(const ReferencePointer<class CommandBuffer>& commandBuffer) = 0;
  virtual const Vector4& GetClearColor() const = 0;
  virtual void SetClearColor(const Vector4& color) = 0;
};
}  // namespace Hydrogen
