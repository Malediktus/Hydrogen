#include "../../Renderer/Renderbuffer.hpp"

namespace Hydrogen::Vulkan {
class VulkanRenderbuffer : public Renderbuffer {
public:
    VulkanRenderbuffer(uint32_t width, uint32_t height, RenderbufferUsageType usageType);
    virtual ~VulkanRenderbuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void Resize(uint32_t width, uint32_t height) override;

    virtual void* GetNative() const override;

private:
    uint32_t m_RendererID;
    uint32_t m_Format;
};
} // namespace Hydrogen::Vulkan
