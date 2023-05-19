#include <Vortex/Platform/OpenGL/OpenGLTexture.hpp>
#include <Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

using namespace Vortex::OpenGL;

OpenGLTexture2D::OpenGLTexture2D(const int width, const int height, Texture2D::Texture2DUsageType usageType) {
    ZoneScoped;
    m_Width = width;
    m_Height = height;
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    GLenum internalFormat = GL_RGBA8;
    GLenum format = GL_RGBA;

    switch (usageType) {
    case Texture2D::Texture2DUsageType::Color:
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
        break;
    case Texture2D::Texture2DUsageType::Depth:
        internalFormat = GL_DEPTH_COMPONENT32F;
        format = GL_DEPTH_COMPONENT;
        break;
    case Texture2D::Texture2DUsageType::Stencil:
        internalFormat = GL_STENCIL_INDEX8;
        format = GL_STENCIL_INDEX;
        break;
    case Texture2D::Texture2DUsageType::DepthStencil:
        internalFormat = GL_DEPTH24_STENCIL8;
        format = GL_DEPTH_STENCIL;
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glCheckError();
    spdlog::trace("Created OpenGL texture2D from file (ID: {})", m_RendererID);
}

OpenGLTexture2D::OpenGLTexture2D(const int width, const int height, const void* data, Texture2D::Texture2DUsageType usageType) {
    ZoneScoped;
    m_Width = width;
    m_Height = height;
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLenum internalFormat = GL_RGBA8;
    GLenum format = GL_RGBA;

    switch (usageType) {
    case Texture2D::Texture2DUsageType::Color:
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
        break;
    case Texture2D::Texture2DUsageType::Depth:
        internalFormat = GL_DEPTH_COMPONENT32F;
        format = GL_DEPTH_COMPONENT;
        break;
    case Texture2D::Texture2DUsageType::Stencil:
        internalFormat = GL_STENCIL_INDEX8;
        format = GL_STENCIL_INDEX;
        break;
    case Texture2D::Texture2DUsageType::DepthStencil:
        internalFormat = GL_DEPTH24_STENCIL8;
        format = GL_DEPTH_STENCIL;
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glCheckError();
    spdlog::trace("Created OpenGL texture2D from file (ID: {})", m_RendererID);
}

OpenGLTexture2D::~OpenGLTexture2D() {
    ZoneScoped;
    glDeleteTextures(1, &m_RendererID);
    glCheckError();
    spdlog::trace("Deleted OpenGL texture2D (ID: {})", m_RendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
    ZoneScoped;
    VT_ASSERT(slot <= 31, "Texture slots over 32 are not supported");
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glCheckError();
    spdlog::trace("Bound OpenGL texture2D (ID: {})", m_RendererID);
}

void OpenGLTexture2D::Resize(uint32_t width, uint32_t height) {
    ZoneScoped;
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    m_Width = width;
    m_Height = height;
    glCheckError();
    spdlog::trace("Resized OpenGL texture2D (ID: {})", m_RendererID);
}

void* OpenGLTexture2D::GetNative() const {
    return (void*) &m_RendererID;
}
