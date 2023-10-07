#pragma once

#include "../Core/Memory.hpp"

namespace Hydrogen {
class SwapChain {
 public:
  virtual ~SwapChain() = default;
  virtual void AcquireNextImage(const ReferencePointer<class CommandBuffer>& commandBuffer) = 0;

  static ReferencePointer<SwapChain> Create(const ReferencePointer<class RenderWindow>& window, const ReferencePointer<class RenderDevice>& renderDevice, bool verticalSync);
};
}  // namespace Hydrogen
