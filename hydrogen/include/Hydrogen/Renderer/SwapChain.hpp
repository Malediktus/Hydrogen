#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "RenderDevice.hpp"
#include "Semaphore.hpp"

namespace Hydrogen {
class SwapChain {
 public:
  virtual ~SwapChain() = default;

  virtual void AcquireNextImage(const ReferencePointer<Semaphore>& semaphore, uint32_t* imageIndex) = 0;

  static ReferencePointer<SwapChain> Create(const ReferencePointer<RenderWindow>& window, const ReferencePointer<RenderDevice>& renderDevice, bool verticalSync);
};
}  // namespace Hydrogen
