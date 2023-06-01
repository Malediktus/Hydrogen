#pragma once

#include "../../Renderer/Texture.hpp"

namespace Hydrogen::Vulkan {
class VulkanTexture2D : public Texture2D {
public:
    VulkanTexture2D(const int width, const int height, Texture2DStorageType storageType);
    VulkanTexture2D(const int width, const int height, const void* data, Texture2DStorageType usageType);
    virtual ~VulkanTexture2D();

    virtual uint32_t GetWidth() const override {
        return m_Width;
    }
    virtual uint32_t GetHeight() const override {
        return m_Height;
    }

    virtual void Bind(uint32_t slot = 0) const override;
    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual void* GetNative() const override;

private:
    uint32_t m_InternalFormat, m_Format, m_Type;
    uint32_t m_Width, m_Height;
    uint32_t m_RendererID;
};
} // namespace Hydrogen::Vulkan
