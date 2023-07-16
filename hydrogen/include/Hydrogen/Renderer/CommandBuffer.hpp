#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "Framebuffer.hpp"
#include "RenderDevice.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include "SwapChain.hpp"

namespace Hydrogen {
class CommandBuffer {
 public:
  virtual ~CommandBuffer() = default;

  virtual void Begin(const ReferencePointer<RenderPass>& renderPass, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Framebuffer>& framebuffer,
                     Vector4 clearColor, uint32_t imageIndex) = 0;
  virtual void End() = 0;

  virtual void CmdDraw(const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Shader>& shader) = 0;

  static ReferencePointer<CommandBuffer> Create(const ReferencePointer<RenderDevice>& renderDevice);
};
}  // namespace Hydrogen
