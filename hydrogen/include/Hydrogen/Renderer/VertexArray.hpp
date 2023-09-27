#pragma once

#include <memory>

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "Buffer.hpp"

namespace Hydrogen {
class VertexArray {
 public:
  virtual ~VertexArray() = default;

  virtual void Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const = 0;

  virtual void AddVertexBuffer(const ReferencePointer<VertexBuffer>& vertexBuffer) = 0;
  virtual void SetIndexBuffer(const ReferencePointer<IndexBuffer>& indexBuffer) = 0;

  virtual const DynamicArray<ReferencePointer<VertexBuffer>>& GetVertexBuffers() const = 0;
  virtual const ReferencePointer<IndexBuffer>& GetIndexBuffer() const = 0;

  static ReferencePointer<VertexArray> Create();
};

}  // namespace Hydrogen
