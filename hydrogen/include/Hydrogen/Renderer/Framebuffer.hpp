#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"

namespace Hydrogen {
class RenderDevice;
class RenderPass;
class SwapChain;

class Framebuffer {
 public:
  virtual ~Framebuffer() = default;

  static ReferencePointer<Framebuffer> Create(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain,
                                              const ReferencePointer<RenderPass>& renderPass);
};
}  // namespace Hydrogen
