#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "RenderDevice.hpp"
#include "SwapChain.hpp"

namespace Hydrogen {
class Framebuffer;

class RenderPass {
 public:
  virtual ~RenderPass() = default;
  virtual void Begin(const ReferencePointer<CommandBuffer>& commandBuffer, const ReferencePointer<Framebuffer>& framebuffer, Vector4 clearColor) = 0;

  static ReferencePointer<RenderPass> Create(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain);
};
}  // namespace Hydrogen
