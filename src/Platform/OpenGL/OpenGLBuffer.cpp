#include <Hydrogen/Platform/OpenGL/OpenGLBuffer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <tracy/Tracy.hpp>
#include <glad/glad.h>

using namespace Hydrogen::OpenGL;

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
    ZoneScoped;
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glCheckError();
    HY_LOG_TRACE("Created OpenGL vertex buffer of type dynamic (ID: {})", m_RendererID);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
    ZoneScoped;
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glCheckError();
    HY_LOG_TRACE("Created OpenGL vertex buffer of type static (ID: {})", m_RendererID);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    ZoneScoped;
    glDeleteBuffers(1, &m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Deleted OpenGL vertex buffer (ID: {})", m_RendererID);
}

void OpenGLVertexBuffer::Bind() const {
    ZoneScoped;
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Bound OpenGL vertex buffer (ID: {})", m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const {
    ZoneScoped;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();
    HY_LOG_TRACE("Unbound OpenGL vertex buffer (ID: {})", m_RendererID);
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
    ZoneScoped;
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glCheckError();
    HY_LOG_TRACE("Set data for OpenGL vertex buffer (ID: {})", m_RendererID);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) {
    ZoneScoped;
    m_Count = count / sizeof(uint32_t);
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glCheckError();
    HY_LOG_TRACE("Created OpenGL index buffer of type static (ID: {})", m_RendererID);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    ZoneScoped;
    glDeleteBuffers(1, &m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Deleted OpenGL index buffer (ID: {})", m_RendererID);
}

void OpenGLIndexBuffer::Bind() const {
    ZoneScoped;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Bound OpenGL index buffer (ID: {})", m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const {
    ZoneScoped;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCheckError();
    HY_LOG_TRACE("Unbound OpenGL index buffer (ID: {})", m_RendererID);
}
