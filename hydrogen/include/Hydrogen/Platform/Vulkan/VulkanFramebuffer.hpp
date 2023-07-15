#include <unordered_map>

#include "../../Renderer/Framebuffer.hpp"

namespace Hydrogen::Vulkan {
class VulkanFramebuffer : public Framebuffer {
 public:
  VulkanFramebuffer(const ReferencePointer<RenderWindow>& window);
  virtual ~VulkanFramebuffer();

  virtual void Bind() const override;
  virtual void BindWrite() const override;
  virtual void BindRead() const override;
  virtual void Unbind() const override;

  virtual void SetDrawBuffers(const std::vector<std::pair<FramebufferAttachment, uint32_t>>& attachments) override;

  virtual void AttachColorTexture(const ReferencePointer<Texture2D>& texture) override;
  virtual void AttachDepthTexture(const ReferencePointer<Texture2D>& texture) override;
  virtual void AttachStencilTexture(const ReferencePointer<Texture2D>& texture) override;
  virtual void AttachDepthStencilTexture(const ReferencePointer<Texture2D>& texture) override;

 private:
  uint32_t m_RendererID;
  uint32_t numAttachments;
  uint32_t numColorAttachments = 0, numDepthAttachments = 0, numStencilAttachments = 0, numDepthStencilAttachments = 0;
};
}  // namespace Hydrogen::Vulkan
