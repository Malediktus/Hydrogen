#pragma once

#include "Buffer.hpp"
#include "Core.hpp"

#include <memory>

namespace Vortex {
/**
 * @brief The VertexArray hold VertexBuffers and IndexBuffers and can be rendered.
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class VertexArray {
public:
    enum Primitive {
        Points = 0,
        Lines = 1,
        LineStrip = 2,
        LineLoop = 3,
        Triangles = 4,
        TriangleStrip = 5,
        TriangleFan = 6,
    };

    virtual ~VertexArray() = default;

    virtual VT_API void Bind() const = 0;
    virtual VT_API void Unbind() const = 0;

    virtual VT_API void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
    virtual VT_API void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

    virtual VT_API void SetPrimitive(Primitive primitive) = 0;
    virtual VT_API Primitive GetPrimitive() const = 0;

    virtual VT_API const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
    virtual VT_API const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
};

VT_API std::shared_ptr<VertexArray> VertexArrayCreate();
} // namespace Vortex
