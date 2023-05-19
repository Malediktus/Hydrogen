#pragma once

#include "../Core/Base.hpp"
#include "../Core/Assert.hpp"
#include "../Core/Memory.hpp"
#include "Buffer.hpp"
#include <memory>

namespace Hydrogen {
enum RenderPrimitive {
    Points = 0,
    Lines = 1,
    LineStrip = 2,
    LineLoop = 3,
    Triangles = 4,
    TriangleStrip = 5,
    TriangleFan = 6,
};

class VertexArray {
public:
    virtual ~VertexArray() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const Reference<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(const Reference<IndexBuffer>& indexBuffer) = 0;

    virtual void SetRenderPrimitive(RenderPrimitive renderPrimitive) = 0;
    virtual RenderPrimitive GetRenderPrimitive() const = 0;

    virtual const std::vector<Reference<VertexBuffer>>& GetVertexBuffers() const = 0;
    virtual const Reference<IndexBuffer>& GetIndexBuffer() const = 0;

    static Reference<VertexArray> Create();
};

} // namespace Hydrogen
