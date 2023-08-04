#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"

namespace Hydrogen {
class RenderDevice;
class SwapChain;

class CommandBuffer {
 public:
  virtual ~CommandBuffer() = default;

  virtual void Reset() = 0;
  virtual void Begin() = 0;
  virtual void End(const ReferencePointer<SwapChain> swapChain) = 0;

  virtual void CmdDraw(const ReferencePointer<class VertexBuffer>& vertexBuffer) = 0;
  virtual void CmdSetViewport(const ReferencePointer<SwapChain>& swapChain, uint32_t width = UINT32_MAX, uint32_t height = UINT32_MAX) = 0;
  virtual void CmdSetScissor(const ReferencePointer<SwapChain>& swapChain, int offsetX = 0, int offsetY = 0) = 0;

  static ReferencePointer<CommandBuffer> Create(const ReferencePointer<RenderDevice>& renderDevice);
};
}  // namespace Hydrogen
