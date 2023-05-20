#include <Hydrogen/Platform/OpenGL/OpenGLTexture.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <glad/glad.h>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::OpenGL;

OpenGLTexture2D::OpenGLTexture2D(const int width, const int height, Texture2DStorageType storageType) {
    ZoneScoped;
    m_Width = width;
    m_Height = height;
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    GLenum internalFormat = GL_RGBA8;
    GLenum format = GL_RGBA;

    switch (storageType) {
    case Texture2DStorageType::RGBA8F:
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
        break;
    case Texture2DStorageType::RGBA8I:
        internalFormat = GL_RGBA8I;
        format = GL_RGBA;
        break;
    case Texture2DStorageType::RGBA16F:
        internalFormat = GL_RGBA16F;
        format = GL_RGBA;
        break;
    case Texture2DStorageType::RGBA16I:
        internalFormat = GL_RGBA16I;
        format = GL_RGBA;
        break;
    case Texture2DStorageType::RGBA32F:
        internalFormat = GL_RGBA32F;
        format = GL_RGBA;
        break;
    case Texture2DStorageType::RGBA32I:
        internalFormat = GL_RGBA32I;
        format = GL_RGBA;
        break;
    case Texture2DStorageType::SRGBA8F:
        internalFormat = GL_SRGB8_ALPHA8;
        format = GL_SRGB_ALPHA;
        break;
    case Texture2DStorageType::RGB8F:
        internalFormat = GL_RGB8;
        format = GL_RGB;
        break;
    case Texture2DStorageType::RGB8I:
        internalFormat = GL_RGB8I;
        format = GL_RGB;
        break;
    case Texture2DStorageType::RGB16F:
        internalFormat = GL_RGB16F;
        format = GL_RGB;
        break;
    case Texture2DStorageType::RGB16I:
        internalFormat = GL_RGB16I;
        format = GL_RGB;
        break;
    case Texture2DStorageType::RGB32F:
        internalFormat = GL_RGB32F;
        format = GL_RGB;
        break;
    case Texture2DStorageType::RGB32I:
        internalFormat = GL_RGB32I;
        format = GL_RGB;
        break;
    case Texture2DStorageType::SRGB8F:
        internalFormat = GL_SRGB8;
        format = GL_SRGB;
        break;
    case Texture2DStorageType::Depth:
        internalFormat = GL_DEPTH_COMPONENT32F;
        format = GL_DEPTH_COMPONENT;
        break;
    case Texture2DStorageType::Stencil:
        internalFormat = GL_STENCIL_INDEX8;
        format = GL_STENCIL_INDEX;
        break;
    case Texture2DStorageType::DepthStencil:
        internalFormat = GL_DEPTH24_STENCIL8;
        format = GL_DEPTH_STENCIL;
        break;
    default:
        HY_INVOKE_ERROR("Invalid Texture2DStorageType");
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glCheckError();
    HY_LOG_TRACE("Created OpenGL texture2D from file (ID: {})", m_RendererID);
}

OpenGLTexture2D::OpenGLTexture2D(const int width, const int height, const void* data, Texture2DStorageType storageType) {
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

    switch (storageType) {
    case Texture2DStorageType::RGBA8F:
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
        break;
    case Texture2DStorageType::RGBA8I:
        internalFormat = GL_RGBA8I;
        format = GL_RGBA;
        break;
    case Texture2DStorageType::RGBA16F:
        internalFormat = GL_RGBA16F;
        format = GL_RGBA;
        break;
    case Texture2DStorageType::RGBA16I:
        internalFormat = GL_RGBA16I;
        format = GL_RGBA;
        break;
    case Texture2DStorageType::RGBA32F:
        internalFormat = GL_RGBA32F;
        format = GL_RGBA;
        break;
    case Texture2DStorageType::RGBA32I:
        internalFormat = GL_RGBA32I;
        format = GL_RGBA;
        break;
    case Texture2DStorageType::SRGBA8F:
        internalFormat = GL_SRGB8_ALPHA8;
        format = GL_SRGB_ALPHA;
        break;
    case Texture2DStorageType::RGB8F:
        internalFormat = GL_RGB8;
        format = GL_RGB;
        break;
    case Texture2DStorageType::RGB8I:
        internalFormat = GL_RGB8I;
        format = GL_RGB;
        break;
    case Texture2DStorageType::RGB16F:
        internalFormat = GL_RGB16F;
        format = GL_RGB;
        break;
    case Texture2DStorageType::RGB16I:
        internalFormat = GL_RGB16I;
        format = GL_RGB;
        break;
    case Texture2DStorageType::RGB32F:
        internalFormat = GL_RGB32F;
        format = GL_RGB;
        break;
    case Texture2DStorageType::RGB32I:
        internalFormat = GL_RGB32I;
        format = GL_RGB;
        break;
    case Texture2DStorageType::SRGB8F:
        internalFormat = GL_SRGB8;
        format = GL_SRGB;
        break;
    case Texture2DStorageType::Depth:
        internalFormat = GL_DEPTH_COMPONENT32F;
        format = GL_DEPTH_COMPONENT;
        break;
    case Texture2DStorageType::Stencil:
        internalFormat = GL_STENCIL_INDEX8;
        format = GL_STENCIL_INDEX;
        break;
    case Texture2DStorageType::DepthStencil:
        internalFormat = GL_DEPTH24_STENCIL8;
        format = GL_DEPTH_STENCIL;
        break;
    default:
        HY_INVOKE_ERROR("Invalid Texture2DStorageType");
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glCheckError();
    HY_LOG_TRACE("Created OpenGL texture2D from file (ID: {})", m_RendererID);
}

OpenGLTexture2D::~OpenGLTexture2D() {
    ZoneScoped;
    glDeleteTextures(1, &m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Deleted OpenGL texture2D (ID: {})", m_RendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
    ZoneScoped;
    HY_ASSERT(slot <= 31, "Texture slots over 32 are not supported");
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glCheckError();
    HY_LOG_TRACE("Bound OpenGL texture2D (ID: {})", m_RendererID);
}

void OpenGLTexture2D::Resize(uint32_t width, uint32_t height) {
    ZoneScoped;
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    m_Width = width;
    m_Height = height;
    glCheckError();
    HY_LOG_TRACE("Resized OpenGL texture2D (ID: {})", m_RendererID);
}

void* OpenGLTexture2D::GetNative() const {
    return (void*) &m_RendererID;
}
