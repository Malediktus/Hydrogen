#pragma once

#include <cstdint>

#include "../Core/Memory.hpp"

namespace Hydrogen {
class CommandBuffer {
 public:
  virtual ~CommandBuffer() = default;

  virtual void Reset() = 0;
  virtual void Begin() = 0;
  virtual void End() = 0;

  virtual void CmdUploadResources() = 0;
  virtual void CmdDisplayImage() = 0;
  virtual void CmdDraw(const ReferencePointer<class VertexBuffer>& vertexBuffer) = 0;
  virtual void CmdDrawIndexed(const ReferencePointer<class VertexArray>& vertexArray) = 0;
  virtual void CmdSetViewport(uint32_t width = UINT32_MAX, uint32_t height = UINT32_MAX) = 0;
  virtual void CmdSetScissor(int offsetX = 0, int offsetY = 0) = 0;
  virtual void CmdDrawImGuiDrawData(const ReferencePointer<class Shader>& shader = nullptr) = 0;

  static ReferencePointer<CommandBuffer> Create(const ReferencePointer<class RenderWindow>& window);
};
}  // namespace Hydrogen
