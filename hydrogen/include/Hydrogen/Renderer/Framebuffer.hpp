#pragma once

#include "../Core/Memory.hpp"
#include "../Math/Math.hpp"

namespace Hydrogen {
class Framebuffer {
 public:
  virtual ~Framebuffer() = default;

  static ReferencePointer<Framebuffer> Create(const ReferencePointer<class RenderDevice>& renderDevice, const ReferencePointer<class SwapChain>& swapChain);

  virtual void Begin(Vector4 clearColor, const ReferencePointer<class CommandBuffer>& commandBuffer) = 0;
  virtual void End(const ReferencePointer<class CommandBuffer>& commandBuffer) = 0;
};
}  // namespace Hydrogen
