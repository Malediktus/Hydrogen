#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"

namespace Hydrogen {
class RenderDevice;
class CommandBuffer;
class RenderWindow;

class SwapChain {
 public:
  virtual ~SwapChain() = default;
  virtual void AcquireNextImage(const ReferencePointer<CommandBuffer>& commandBuffer) = 0;

  static ReferencePointer<SwapChain> Create(const ReferencePointer<RenderWindow>& window, const ReferencePointer<RenderDevice>& renderDevice, bool verticalSync);
};
}  // namespace Hydrogen
