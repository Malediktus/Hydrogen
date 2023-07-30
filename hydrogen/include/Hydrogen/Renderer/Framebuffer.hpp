#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "RenderDevice.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"

namespace Hydrogen {
class Framebuffer {
 public:
  virtual ~Framebuffer() = default;

  static ReferencePointer<Framebuffer> Create(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain,
                                              const ReferencePointer<RenderPass>& renderPass);
};
}  // namespace Hydrogen
