#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "RenderDevice.hpp"
#include "SwapChain.hpp"

namespace Hydrogen {
class RenderPass {
 public:
  virtual ~RenderPass() = default;

  static ReferencePointer<RenderPass> Create(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain);
};
}  // namespace Hydrogen
