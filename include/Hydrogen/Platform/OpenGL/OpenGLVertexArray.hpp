#pragma once

#include "../../Renderer/VertexArray.hpp"

namespace Hydrogen::OpenGL {
class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const Reference<VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(const Reference<IndexBuffer>& indexBuffer) override;

    virtual void SetRenderPrimitive(RenderPrimitive primitive) override {
        m_Primitive = primitive;
    }

    virtual RenderPrimitive GetRenderPrimitive() const override {
        return m_Primitive;
    }

    virtual const std::vector<Reference<VertexBuffer>>& GetVertexBuffers() const override {
        return m_VertexBuffers;
    }
    virtual const Reference<IndexBuffer>& GetIndexBuffer() const override {
        return m_IndexBuffer;
    }

private:
    uint32_t m_RendererID;
    RenderPrimitive m_Primitive;
    std::vector<Reference<VertexBuffer>> m_VertexBuffers;
    Reference<IndexBuffer> m_IndexBuffer;
};
} // namespace Hydrogen::OpenGL
