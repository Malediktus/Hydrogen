#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "Framebuffer.hpp"
#include "RenderDevice.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"

namespace Hydrogen {
class CommandBuffer {
 public:
  virtual ~CommandBuffer() = default;

  virtual void Reset() = 0;
  virtual void Begin(const ReferencePointer<RenderPass>& renderPass, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Framebuffer>& framebuffer,
                     Vector4 clearColor) = 0;
  virtual void End(const ReferencePointer<SwapChain> swapChain) = 0;

  virtual void CmdDraw(const ReferencePointer<class VertexBuffer>& vertexBuffer) = 0;
  virtual void CmdSetViewport(const ReferencePointer<SwapChain>& swapChain, uint32_t width = UINT32_MAX, uint32_t height = UINT32_MAX) = 0;

  static ReferencePointer<CommandBuffer> Create(const ReferencePointer<RenderDevice>& renderDevice);
};
}  // namespace Hydrogen
