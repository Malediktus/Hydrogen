#include <Vortex/Platform/OpenGL/OpenGLBuffer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>
#include <glad/glad.h>

using namespace Vortex::OpenGL;

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
    ZoneScoped;
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glCheckError();
    spdlog::trace("Created OpenGL vertex buffer of type dynamic (ID: {})", m_RendererID);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
    ZoneScoped;
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glCheckError();
    spdlog::trace("Created OpenGL vertex buffer of type static (ID: {})", m_RendererID);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    ZoneScoped;
    glDeleteBuffers(1, &m_RendererID);
    glCheckError();
    spdlog::trace("Deleted OpenGL vertex buffer (ID: {})", m_RendererID);
}

void OpenGLVertexBuffer::Bind() const {
    ZoneScoped;
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glCheckError();
    spdlog::trace("Bound OpenGL vertex buffer (ID: {})", m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const {
    ZoneScoped;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();
    spdlog::trace("Unbound OpenGL vertex buffer (ID: {})", m_RendererID);
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
    ZoneScoped;
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glCheckError();
    spdlog::trace("Set data for OpenGL vertex buffer (ID: {})", m_RendererID);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) {
    ZoneScoped;
    m_Count = count / sizeof(uint32_t);
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glCheckError();
    spdlog::trace("Created OpenGL index buffer of type static (ID: {})", m_RendererID);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    ZoneScoped;
    glDeleteBuffers(1, &m_RendererID);
    glCheckError();
    spdlog::trace("Deleted OpenGL index buffer (ID: {})", m_RendererID);
}

void OpenGLIndexBuffer::Bind() const {
    ZoneScoped;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glCheckError();
    spdlog::trace("Bound OpenGL index buffer (ID: {})", m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const {
    ZoneScoped;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCheckError();
    spdlog::trace("Unbound OpenGL index buffer (ID: {})", m_RendererID);
}
