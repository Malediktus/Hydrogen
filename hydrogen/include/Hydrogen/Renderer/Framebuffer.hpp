#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "../Math/Math.hpp"

namespace Hydrogen {
class RenderDevice;
class RenderPass;
class SwapChain;
class CommandBuffer;

class Framebuffer {
 public:
  virtual ~Framebuffer() = default;

  static ReferencePointer<Framebuffer> Create(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain);

  virtual void Bind(const ReferencePointer<CommandBuffer>& commandBuffer) = 0;
  virtual const Vector4& GetClearColor() const = 0;
  virtual void SetClearColor(const Vector4& color) = 0;
};
}  // namespace Hydrogen
