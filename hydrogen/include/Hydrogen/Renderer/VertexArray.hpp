#pragma once

#include "../Core/Memory.hpp"

namespace Hydrogen {
class VertexArray {
 public:
  virtual ~VertexArray() = default;

  virtual void Bind() const = 0;

  virtual void AddVertexBuffer(const ReferencePointer<class VertexBuffer>& vertexBuffer) = 0;
  virtual void SetIndexBuffer(const ReferencePointer<class IndexBuffer>& indexBuffer) = 0;

  virtual const DynamicArray<ReferencePointer<class VertexBuffer>>& GetVertexBuffers() const = 0;
  virtual const ReferencePointer<class IndexBuffer>& GetIndexBuffer() const = 0;

  static ReferencePointer<class VertexArray> Create();
};

}  // namespace Hydrogen
