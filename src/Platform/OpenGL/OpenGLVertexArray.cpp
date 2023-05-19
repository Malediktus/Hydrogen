#include <Hydrogen/Platform/OpenGL/OpenGLVertexArray.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <tracy/Tracy.hpp>
#include <glad/glad.h>

using namespace Hydrogen::OpenGL;

namespace Hydrogen::Utils {
static GLenum ShaderDataTypeToOpenGLBaseType(Hydrogen::ShaderDataType type) {
    switch (type) {
    case Hydrogen::ShaderDataType::Float:
        return GL_FLOAT;
    case Hydrogen::ShaderDataType::Float2:
        return GL_FLOAT;
    case Hydrogen::ShaderDataType::Float3:
        return GL_FLOAT;
    case Hydrogen::ShaderDataType::Float4:
        return GL_FLOAT;
    case Hydrogen::ShaderDataType::Mat3:
        return GL_FLOAT;
    case Hydrogen::ShaderDataType::Mat4:
        return GL_FLOAT;
    case Hydrogen::ShaderDataType::Int:
        return GL_INT;
    case Hydrogen::ShaderDataType::Int2:
        return GL_INT;
    case Hydrogen::ShaderDataType::Int3:
        return GL_INT;
    case Hydrogen::ShaderDataType::Int4:
        return GL_INT;
    case Hydrogen::ShaderDataType::Bool:
        return GL_BOOL;
    default:
        HY_ASSERT_CHECK(false, "Invalid shader data type");
    }
    return 0;
}
} // namespace Hydrogen::Utils

OpenGLVertexArray::OpenGLVertexArray() {
    ZoneScoped;
    glGenVertexArrays(1, &m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Created OpenGL vertex array (ID: {})", m_RendererID);
    m_Primitive = RenderPrimitive::Triangles;
}

OpenGLVertexArray::~OpenGLVertexArray() {
    ZoneScoped;
    glDeleteVertexArrays(1, &m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Deleted OpenGL vertex array (ID: {})", m_RendererID);
}

void OpenGLVertexArray::Bind() const {
    ZoneScoped;
    glBindVertexArray(m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Bound OpenGL vertex array (ID: {})", m_RendererID);
}

void OpenGLVertexArray::Unbind() const {
    ZoneScoped;
    glBindVertexArray(0);
    glCheckError();
    HY_LOG_TRACE("Unbount OpenGL vertex array (ID: {})", m_RendererID);
}

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
    ZoneScoped;
    HY_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "At lease one value is needed in vertex buffer layout");

    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();

    uint32_t index = 0;
    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, element.GetComponentCount(), Hydrogen::Utils::ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(), (const void*) element.Offset);
        index++;
    }
    m_VertexBuffers.push_back(vertexBuffer);

    glCheckError();
    HY_LOG_TRACE("Attached vertex buffer to OpenGL vertex array (ID: {})", m_RendererID);
}

void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
    ZoneScoped;
    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();
    m_IndexBuffer = indexBuffer;

    glCheckError();
    HY_LOG_TRACE("Set index buffer of OpenGL vertex array (ID: {})", m_RendererID);
}
